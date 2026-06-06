// Gof2BSim.java — BSim cross-binary bulk naming for the GOF2 decomp.
// Pipeline validated: create H2 DB (medium_32 / ARM32) -> GenSignatures -> InsertRequest ->
// QueryNearest -> apply names. Self-query test matched 150/150 at similarity 1.000.
//
// Usage (run in the Ghidra GUI / headless where it can run for several minutes — signature
// generation decompiles every function, so a full ~7500-function pass is a batch job):
//   1) Build the reference DB from the richly-named binary (open android_2.0.16, then run):
//        Gof2BSim  build  /tmp/gof2.bsim
//   2) Query+name the target (open ios_1.1.4_HD, then run):
//        Gof2BSim  query  /tmp/gof2.bsim  0.70  apply
//      -> for each target FUN_ whose nearest DB match (sim>=thresh) has a real name, renames it.
//
// Note: Android(clang) -> iOS(llvm-gcc) is a CROSS-COMPILER match — BSim's harder case; validate
// the similarity threshold on first use (raise it if false positives appear). Same-compiler pairs
// (e.g. iOS 1.0.5 -> iOS 1.1.4) match at much higher confidence.
//
// @category GalaxyOnFire2

import ghidra.app.script.GhidraScript;
import ghidra.features.bsim.query.*;
import ghidra.features.bsim.query.protocol.*;
import ghidra.features.bsim.query.description.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.util.*;

public class Gof2BSim extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) { println("args: build <db> | query <db> <simthresh> [apply]"); return; }
        String mode = a[0], dbpath = a[1];
        BSimServerInfo si = new BSimServerInfo(dbpath);
        try (FunctionDatabase db = BSimClientFactory.buildClient(si, false)) {
            if (mode.equals("build")) build(db, dbpath);
            else if (mode.equals("query")) query(db, Double.parseDouble(a[2]), a.length > 3 && a[3].equals("apply"));
        }
    }

    private void build(FunctionDatabase db, String dbpath) throws Exception {
        if (!new File(dbpath + BSimServerInfo.H2_FILE_EXTENSION).exists()) {
            CreateDatabase c = new CreateDatabase();
            c.info = new DatabaseInformation();
            c.info.databasename = "gof2"; c.config_template = "medium_32"; c.info.trackcallgraph = true;
            if (c.execute(db) == null) throw new Exception("create: " + db.getLastError().message);
        }
        db.initialize();
        DatabaseInformation info = db.getInfo();
        GenSignatures g = new GenSignatures(info.trackcallgraph);
        g.setVectorFactory(db.getLSHVectorFactory());
        g.addExecutableCategories(info.execats); g.addFunctionTags(info.functionTags);
        g.addDateColumnName(info.dateColumnName);
        g.openProgram(currentProgram, null, null, null, null, null);
        FunctionManager fm = currentProgram.getFunctionManager();
        g.scanFunctions(fm.getFunctions(true), fm.getFunctionCount(), monitor);
        DescriptionManager man = g.getDescriptionManager();
        man.listAllFunctions().forEachRemaining(fd -> fd.sortCallgraph());
        InsertRequest ins = new InsertRequest(); ins.manage = man;
        if (ins.execute(db) == null) throw new Exception("insert: " + db.getLastError().message);
        println("ingested " + man.numFunctions() + " signatures from " + currentProgram.getName());
    }

    private void query(FunctionDatabase db, double thresh, boolean apply) throws Exception {
        db.initialize();
        GenSignatures g = new GenSignatures(false);
        g.setVectorFactory(db.getLSHVectorFactory());
        g.openProgram(currentProgram, null, null, null, null, null);
        FunctionManager fm = currentProgram.getFunctionManager();
        g.scanFunctions(fm.getFunctions(true), fm.getFunctionCount(), monitor);
        QueryNearest q = new QueryNearest();
        q.manage = g.getDescriptionManager(); q.max = 1; q.thresh = thresh; q.signifthresh = 0.0;
        ResponseNearest r = q.execute(db);
        if (r == null) throw new Exception("query: " + db.getLastError().message);
        int renamed = 0, candidates = 0;
        int tx = currentProgram.startTransaction("bsim-apply");
        try {
            for (SimilarityResult sr : r.result) {
                Iterator<SimilarityNote> ni = sr.iterator();
                if (!ni.hasNext()) continue;
                SimilarityNote n = ni.next();
                String srcName = n.getFunctionDescription().getFunctionName();
                String tgtName = sr.getBase().getFunctionName();
                boolean tgtUnnamed = tgtName.startsWith("FUN_") || tgtName.startsWith("SUB_");
                boolean srcReal = !(srcName.startsWith("FUN_") || srcName.startsWith("SUB_"));
                if (tgtUnnamed && srcReal) {
                    candidates++;
                    if (apply) {
                        ghidra.program.model.listing.Function f =
                            fm.getFunctionAt(sr.getBase().getAddress());
                        if (f != null) { f.setName(srcName, SourceType.ANALYSIS); renamed++; }
                    }
                }
            }
        } finally { currentProgram.endTransaction(tx, true); }
        println("query results=" + r.result.size() + " name-candidates=" + candidates +
                (apply ? " renamed=" + renamed : " (dry-run; pass 'apply' to rename)"));
    }
}

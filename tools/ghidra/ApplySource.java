// Apply the verified source's types/signatures/learnings onto the loaded original
// program, keyed by mangled symbol name (resolved via Ghidra's own symbol table --
// no _work/ files). Idempotent; safe to re-run. Java GhidraScript (this Ghidra build
// runs Java, not PyGhidra). Reads sibling JSON from the repo (pinned path below).
//
// Args (space-separated; default "structs globals comments"):
//   structs globals comments protos force
//   - protos  = apply all ~5.5k function prototypes (slow, opt-in)
//   - force   = overwrite even structs Ghidra already models richer (default: keep them)
//@category GOF2
import java.io.FileReader;
import java.util.*;

import com.google.gson.*;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.symbol.*;

public class ApplySource extends GhidraScript {
    static final String HERE = "/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp/tools/ghidra";
    // Structs the verified source models authoritatively better than Ghidra's harvest
    // (campaign-proven math/layout sizes). Everything else that Ghidra already populates
    // is LEFT ALONE -- Ghidra's harvested field names are richer and a sparse-source
    // overwrite would regress them. We only fill OPAQUE (<=1 byte) Ghidra structs.
    static final Set<String> FORCE_FIX = new HashSet<>(Arrays.asList(
        "Matrix", "Vector", "Quaternion"));
    // typedef aliases DWARF emits that Ghidra has under another name / no name
    static final Map<String, String> ALIAS = new HashMap<>();
    static {
        for (String s : new String[] {"Vec3", "V3", "Triple"}) ALIAS.put(s, "Vector");
        ALIAS.put("IntArray", "Array<int>");
        ALIAS.put("FILE", "void *");
        // function-pointer typedefs -> opaque pointer
        for (String s : new String[] {"ConfigTokenReadFunction", "LoadingCallback_t",
                "ImageCallback", "CheatFunc", "ResumeCallback_t", "QuitCallback_t",
                "InitializeCallback", "DestroyCallback"})
            ALIAS.put(s, "void *");
    }
    DataTypeManager dtm;
    SymbolTable st;
    Listing lst;
    Map<String, Integer> unresolved = new HashMap<>();
    int symMissing = 0, structsApplied = 0, structsKept = 0, globalsApplied = 0,
        protosApplied = 0, typeComments = 0, symComments = 0;
    List<String> resizes = new ArrayList<>();

    public void run() throws Exception {
        dtm = currentProgram.getDataTypeManager();
        st = currentProgram.getSymbolTable();
        lst = currentProgram.getListing();
        Set<String> phases = new HashSet<>(Arrays.asList(getScriptArgs()));
        if (phases.isEmpty())
            phases.addAll(Arrays.asList("structs", "globals", "comments"));
        boolean force = phases.contains("force");

        JsonObject src = new JsonParser().parse(
            new FileReader(HERE + "/source_types.json")).getAsJsonObject();
        JsonObject learn = new JsonParser().parse(
            new FileReader(HERE + "/learnings.json")).getAsJsonObject();

        int tx = currentProgram.startTransaction("apply_source");
        try {
            if (phases.contains("structs")) {
                println("== structs ==");
                applyStructs(src.getAsJsonObject("structs"), force);
            }
            if (phases.contains("globals")) {
                println("== globals ==");
                applyGlobals(src.getAsJsonObject("globals"));
            }
            if (phases.contains("comments")) {
                println("== comments ==");
                applyComments(learn);
            }
            if (phases.contains("protos")) {
                println("== protos (slow) ==");
                applyProtos(src.getAsJsonObject("functions"));
            }
        } finally {
            currentProgram.endTransaction(tx, true);
        }
        println("APPLIED structs=" + structsApplied + " kept=" + structsKept
            + " globals=" + globalsApplied + " protos=" + protosApplied
            + " typeComments=" + typeComments + " symComments=" + symComments);
        println("symbols missing in program: " + symMissing);
        println("struct resizes: " + resizes);
        List<Map.Entry<String, Integer>> u = new ArrayList<>(unresolved.entrySet());
        u.sort((a, b) -> b.getValue() - a.getValue());
        println("top unresolved types: " + u.subList(0, Math.min(15, u.size())));
    }

    DataType prim(String s) {
        switch (s) {
            case "void": return VoidDataType.dataType;
            case "bool": return BooleanDataType.dataType;
            case "char": return CharDataType.dataType;
            case "signed char": return SignedCharDataType.dataType;
            case "unsigned char": return Undefined1DataType.dataType;
            case "short": return ShortDataType.dataType;
            case "unsigned short": return UnsignedShortDataType.dataType;
            case "int": case "long int": case "jint": case "jsize":
                return IntegerDataType.dataType;
            case "unsigned int": case "long unsigned int": case "unsigned long":
                return UnsignedIntegerDataType.dataType;
            case "long": case "long long": case "long long int": case "jlong":
                return LongLongDataType.dataType;
            case "unsigned long long": case "long long unsigned int":
                return UnsignedLongLongDataType.dataType;
            case "float": case "jfloat": return FloatDataType.dataType;
            case "double": case "long double": case "jdouble":
                return DoubleDataType.dataType;
            case "jboolean": return Undefined1DataType.dataType;
            case "jbyte": return SignedCharDataType.dataType;
            case "jchar": case "jshort": return ShortDataType.dataType;
            case "char16_t": return UnsignedShortDataType.dataType;
            // JNI reference handles are opaque pointers
            case "jobject": case "jclass": case "jstring": case "jthrowable":
            case "jweak": case "jarray": case "jobjectArray": case "jbyteArray":
            case "jintArray": case "jbooleanArray": case "jcharArray":
            case "jshortArray": case "jlongArray": case "jfloatArray":
            case "jdoubleArray": case "jmethodID": case "jfieldID":
                return new PointerDataType(VoidDataType.dataType);
            default: return null;
        }
    }

    DataType findNamed(String name) {
        ArrayList<DataType> out = new ArrayList<>();
        dtm.findDataTypes(name, out);
        return out.isEmpty() ? null : out.get(0);
    }

    DataType resolveType(String s) {
        s = s.trim();
        if (s.endsWith("*"))
            return new PointerDataType(resolveType(s.substring(0, s.length() - 1)));
        if (s.endsWith("]")) {
            int i = s.lastIndexOf('[');
            DataType elem = resolveType(s.substring(0, i));
            int n;
            try {
                n = Integer.parseInt(s.substring(i + 1, s.length() - 1).trim());
            } catch (Exception e) {
                n = 1;
            }
            if (n < 1) n = 1;
            return new ArrayDataType(elem, n, elem.getLength());
        }
        DataType p = prim(s);
        if (p != null) return p;
        if (ALIAS.containsKey(s)) return resolveType(ALIAS.get(s));
        DataType d = findNamed(s);
        if (d != null) return d;
        unresolved.merge(s, 1, Integer::sum);
        return Undefined1DataType.dataType;
    }

    Address addrOf(String mangled) {
        List<Symbol> gs = st.getGlobalSymbols(mangled);
        if (!gs.isEmpty()) return gs.get(0).getAddress();
        SymbolIterator it = st.getSymbols(mangled);
        if (it.hasNext()) return it.next().getAddress();
        return null;
    }

    Map<Integer, String> existingNames(String name) {
        Map<Integer, String> out = new HashMap<>();
        DataType dt = findNamed(name);
        if (dt instanceof Structure) {
            for (DataTypeComponent c : ((Structure) dt).getComponents()) {
                String fn = c.getFieldName();
                if (fn != null && !fn.startsWith("field_") && !fn.startsWith("unnamed"))
                    out.put(c.getOffset(), fn);
            }
        }
        return out;
    }

    void applyStructs(JsonObject structs, boolean force) {
        for (Map.Entry<String, JsonElement> e : structs.entrySet()) {
            String name = e.getKey();
            try {
                JsonObject info = e.getValue().getAsJsonObject();
                int size = info.get("size").getAsInt();
                JsonArray members = info.getAsJsonArray("members");
                DataType old = findNamed(name);
                Map<Integer, String> keep = existingNames(name);
                // Safe merge: only (re)build a struct that is NEW, OPAQUE in Ghidra
                // (<=1 byte placeholder), or an explicit campaign-verified fix. Never
                // overwrite a populated Ghidra struct -- its harvested names beat a
                // sparse source rebuild.
                if (old != null && !force && !FORCE_FIX.contains(name)
                        && old.getLength() > 1) {
                    structsKept++;
                    continue;
                }
                CategoryPath cat = (old != null) ? old.getCategoryPath()
                                                 : new CategoryPath("/GOF2");
                StructureDataType s = new StructureDataType(cat, name, size, dtm);
                if (old != null && old.getLength() != size)
                    resizes.add(name + " " + old.getLength() + "->" + size);
                for (JsonElement me : members) {
                    JsonObject m = me.getAsJsonObject();
                    int off = m.get("offset").getAsInt();
                    String mn = m.get("name").getAsString();
                    if (mn.startsWith("field_0x") && keep.containsKey(off))
                        mn = keep.get(off);
                    DataType dt = resolveType(m.get("type").getAsString());
                    if (dt instanceof VoidDataType)  // e.g. member-fn-ptr members -> opaque ptr
                        dt = new PointerDataType(VoidDataType.dataType);
                    if (off + dt.getLength() <= s.getLength())
                        s.replaceAtOffset(off, dt, dt.getLength(), mn, null);
                }
                dtm.addDataType(s, DataTypeConflictHandler.REPLACE_HANDLER);
                structsApplied++;
            } catch (Exception ex) {
                println("[struct] " + name + ": " + ex);
            }
        }
    }

    void applyGlobals(JsonObject globals) {
        for (Map.Entry<String, JsonElement> e : globals.entrySet()) {
            try {
                Address a = addrOf(e.getKey());
                if (a == null) { symMissing++; continue; }
                DataType dt = resolveType(e.getValue().getAsString());
                try {
                    lst.clearCodeUnits(a, a.add(Math.max(dt.getLength() - 1, 0)), false);
                    lst.createData(a, dt);
                    globalsApplied++;
                } catch (Exception ignore) { /* already typed / conflict */ }
            } catch (Exception ex) {
                println("[global] " + e.getKey() + ": " + ex);
            }
        }
    }

    void applyProtos(JsonObject funcs) {
        int n = 0;
        for (Map.Entry<String, JsonElement> e : funcs.entrySet()) {
            Address a = addrOf(e.getKey());
            if (a == null) { symMissing++; continue; }
            Function fn = getFunctionAt(a);
            if (fn == null) continue;
            try {
                JsonObject sig = e.getValue().getAsJsonObject();
                List<Variable> params = new ArrayList<>();
                for (JsonElement pe : sig.getAsJsonArray("params")) {
                    JsonObject p = pe.getAsJsonObject();
                    String ty = p.get("type").getAsString();
                    if (ty.equals("...")) continue;
                    String pn = p.get("name").getAsString();
                    params.add(new ParameterImpl(pn.isEmpty() ? null : pn,
                        resolveType(ty), currentProgram));
                }
                fn.replaceParameters(params,
                    FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true,
                    SourceType.USER_DEFINED);
                fn.setReturnType(resolveType(sig.get("ret").getAsString()),
                    SourceType.USER_DEFINED);
                protosApplied = ++n;
                if (n % 500 == 0) println("  ...protos " + n);
            } catch (Exception ex) {
                if (n < 20) println("[proto] " + e.getKey() + ": " + ex);
            }
        }
    }

    void applyComments(JsonObject learn) {
        JsonObject types = learn.getAsJsonObject("types");
        for (Map.Entry<String, JsonElement> e : types.entrySet()) {
            DataType dt = findNamed(e.getKey());
            if (dt != null) {
                try { dt.setDescription(e.getValue().getAsString()); typeComments++; }
                catch (Exception ex) { println("[type-comment] " + e.getKey() + ": " + ex); }
            }
        }
        JsonObject syms = learn.getAsJsonObject("symbols");
        for (Map.Entry<String, JsonElement> e : syms.entrySet()) {
            Address a = addrOf(e.getKey());
            if (a == null) continue;
            try {
                lst.setComment(a, CodeUnit.PLATE_COMMENT, e.getValue().getAsString());
                symComments++;
            } catch (Exception ex) {
                println("[sym-comment] " + e.getKey() + ": " + ex);
            }
        }
    }
}

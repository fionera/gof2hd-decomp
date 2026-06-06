// Gof2ExtractCtx.java — dump a per-function work-item for the matching loop.
// Writes work/<name>.md with: signature, ELF vaddr, Ghidra decompile (types applied),
// target disassembly, callees, and the ready-to-run gofdiff command.
//
// Usage (Ghidra GUI / headless, with the program open):
//   Gof2ExtractCtx <ghidra_addr_hex>          e.g.  Gof2ExtractCtx b721c
// (Android image base 0x10000: ELF vaddr = ghidra_addr - 0x10000.)
//
// @category GalaxyOnFire2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.Address;
import java.io.*;

public class Gof2ExtractCtx extends GhidraScript {
    static final String OUT = "/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp/work";
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        long ghidraAddr = Long.parseLong(args.length > 0 ? args[0] : "b721c", 16);
        long elf = ghidraAddr - 0x10000;     // Android rebase; for iOS use ghidraAddr as-is
        Address a = toAddr(ghidraAddr);
        Function f = getFunctionContaining(a);
        DecompInterface di = new DecompInterface(); di.openProgram(currentProgram);
        DecompileResults rr = di.decompileFunction(f, 60, monitor);
        String c = rr.getDecompiledFunction() != null ? rr.getDecompiledFunction().getC() : "<failed>";
        StringBuilder dis = new StringBuilder();
        InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (ii.hasNext()) { Instruction in = ii.next(); dis.append("  " + in.getAddressString(false,true) + ": " + in + "\n"); }
        StringBuilder cal = new StringBuilder();
        for (Function ce : f.getCalledFunctions(monitor)) cal.append("  " + ce.getName(true) + "\n");
        String name = f.getName();
        int n = (int) f.getBody().getNumAddresses() + 16;
        String md = "# " + f.getName(true) + "\n\nELF vaddr 0x" + Long.toHexString(elf) +
            " (Ghidra 0x" + Long.toHexString(ghidraAddr) + "), body " + f.getBody().getNumAddresses() +
            " bytes\nSignature: " + f.getSignature() + "\n\n## Ghidra decompile\n```c\n" + c +
            "```\n\n## Target disassembly\n```\n" + dis + "```\n\n## Callees\n" +
            (cal.length() > 0 ? cal.toString() : "  (none)\n") +
            "\n## Verify\n```\npython3 tools/gofdiff.py --so ../_work/bins/android_2.0.16_libgof2hdaa.so" +
            " --vaddr " + Long.toHexString(elf) + " --n " + n + " --obj build/obj/<tu>.o --sym " + name + "\n```\n";
        new File(OUT).mkdirs();
        try (PrintWriter w = new PrintWriter(new FileWriter(OUT + "/" + name + ".md"))) { w.print(md); }
        println("wrote work/" + name + ".md");
    }
}

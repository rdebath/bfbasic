//====================================================================
// BFC -- BF to MS-DOS .COM program compiler
// Filename : bfc.java
// Language : Java 1.2+
// Version  : 0.10
// Copyright: (C) 2003 by Jeffry Johnston
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation. See the file LICENSE
// for more details.
//====================================================================

import java.util.*;
import java.io.*;

//********************************************************************
// bfc
//********************************************************************
public class bfc {
  public static final String VERSION = "0.10";

  static final int START = 0x0100;
  static final int STACK_MAX = 1000;
  static final int MEMORY = 0xF0;
  static DataInputStream in = null;
  static DataOutputStream out = null;
  static int[] buffer = new int[MEMORY * 0x100];

  //------------------------------------------------------------------
  // main
  //------------------------------------------------------------------
  public static void main(String[] cmdline) {
    // display copyright text
    System.out.println("BFC Java BF Compiler, version " + VERSION
                       + ".  Copyright 2003 by Jeffry Johnston.\r\n");

    // process command line
    String f = "", f2 = "";
    int n;
    for (n = 0; ; n++) {
      try {
        if (cmdline[n].equals("-?")) {
          usage();
          System.exit(0);
        } else if (cmdline[n].equalsIgnoreCase("-o")) {
          f2 = cmdline[n+1];
          n++;
        } else {
          f = cmdline[n];
        }
      } catch (ArrayIndexOutOfBoundsException e) {
        break;
      }
    }
    if (f.equals("")) {
      usage();
      errout("Error: No input file given");
    }
    if ((n = f.indexOf('.')) == -1) {
      // create default output filename
      if (f2.equals("")) { f2 = f + ".com"; }
      // add missing ".bf" extension
      f += ".bf";
    } else {
      // create default output filename
      if (f2.equals("")) { f2 = f.substring(0, n) + ".com"; }
    }

    // display filenames
    System.out.println("Input file : " + f);
    System.out.println("Output file: " + f2);

    // open input file
    try {
      in = new DataInputStream(new FileInputStream(f));
    } catch (FileNotFoundException e) {
      errout("Error opening '" + f + "'.  File not found");
    }

    // read input file
    byte[] code = null;
    int cpmax = 0;
    try {
      cpmax = in.available();
      code = new byte[cpmax];
      in.readFully(code);
    } catch (IOException e) {
      errout("Error reading '" + f + "'");
    }

    // close input file
    try {
      in.close();
    } catch (IOException e) {
      errout("Error closing '" + f + "'");
    }

    // (header)
    // XOR CX,CX                        31 C9
    // AGAIN: DEC BX                    4B
    // MOV [BX],CH                      88 2F
    // CMP BH,##                        80 FF ##
    // JNZ AGAIN                        75 F8
    int ip = START;
    ip = write(ip, 10, new int[] {0x31, 0xC9, 0x4B, 0x88, 0x2F,
                                  0x80, 0xFF, MEMORY - 1, 0x75, 0xF8});

    int pm = 0, lr = 0, sp = 0, temp;
    int[] stack = new int[STACK_MAX];
    ArrayList al = new ArrayList(); // Java 1.2

    // main loop
    for (int cp = 0; cp < cpmax; cp++) {
      if ("+-<>,.[]".indexOf(code[cp]) == -1) {
        continue;
      }
      if ((pm != 0) && (((code[cp] != '+') && (code[cp] != '-'))
          || (pm == 255 || pm == -255))) {
        // output +-
        if (pm == 1) {
          // +
          // INC BYTE PTR [BX]          FE 07
          ip = write(ip, 2, new int[] {0xFE, 0x07});
        } else if (pm == -1) {
          // -
          // DEC BYTE PTR [BX]          FE 0F
          ip = write(ip, 2, new int[] {0xFE, 0x0F});
        } else if (pm > 0) {
          // +
          // ADD BYTE PTR [BX],##       80 07 ##
          ip = write(ip, 3, new int[] {0x80, 0x07, pm});
        } else {
          // +
          // SUB BYTE PTR [BX],##       80 2F ##
          ip = write(ip, 3, new int[] {0x80, 0x2F, -pm});
        }
        pm = 0;
      }
      if ((lr != 0) && (code[cp] != '<') && (code[cp] != '>')) {
        // output <>
        if (lr > 0 && lr < 4) {
          // >
          // INC BX                     43
          for (n = 0; n < lr; n++) {
            ip = write(ip, 1, new int[] {0x43});
          }
        } else if (lr < 0 && lr > -4) {
          // <
          // DEC BX                     4B
          for (n = 0; n < -lr; n++) {
            ip = write(ip, 1, new int[] {0x4B});
          }
        } else if (lr > 0) {
          // >
          // ADD BX,####                81 C3 #l #h
          ip = write(ip, 4, new int[] {0x81, 0xC3, lr & 0x00FF, (lr & 0xFF00) >> 8 });
        } else {
          // <
          // SUB BX,####                81 EB #l #h
          ip = write(ip, 4, new int[] {0x81, 0xEB, (-lr) & 0x00FF, ((-lr) & 0xFF00) >> 8 });
        }
        lr = 0;
      }
      switch (code[cp]) {
      case '+':
        pm++;
        break;
      case '-':
        pm--;
        break;
      case '<':
        lr--;
        break;
      case '>':
        lr++;
        break;
      case ',':
        // ,
        // MOV AH,08h                   B4 08
        // INT 21h                      CD 21
        // MOV [BX],AL                  88 07
        ip = write(ip, 6, new int[] {0xB4, 0x08, 0xCD, 0x21, 0x88, 0x07});
        break;
      case '.':
        // .
        // MOV AH,02h                   B4 02
        // MOV DL,[BX]                  8A 17
        // INT 21h                      CD 21
        ip = write(ip, 6, new int[] {0xB4, 0x02, 0x8A, 0x17, 0xCD, 0x21});
        break;
      case '[':
        // [
        // CMP BYTE PTR [BX], 00h       80 3F 00
        // JNZ ...                      75 03
        // JMP ####                     E9(#l #h)
        // save jump address on stack
        stack[sp++] = ip;
        // destination can't be calculated yet (0x00 0x00)
        ip = write(ip, 8, new int[] {0x80, 0x3F, 0x00, 0x75, 0x03, 0xE9, 0x00, 0x00});
        // save ip so destination can be inserted later
        al.add(new Integer(-ip));
        break;
      case ']':
        // ]
        // JMP ####                     E9(#l #h)
        // retrieve jump address from stack
        temp = (stack[--sp] - ip - 3 + 65536) % 65536;
        ip = write(ip, 3, new int[] {0xE9, temp & 0x00FF, (temp & 0xFF00) >> 8});
        // save ip so [ destination can be calculated later
        al.add(new Integer(ip));
        break;
      }
    }

    // resolve jump addresses for ['s
    int dest = 0, fixip, tempn;
    for (n = 0; n < al.size(); n++) {
      fixip = ((Integer) al.get(n)).intValue();
      if (fixip < 0) { // [
        // find matching ]
        tempn = n + 1;
        for (int depth = 1; depth > 0; tempn++) {
          dest = ((Integer) al.get(tempn)).intValue();
          if (dest < 0) { depth++; } // [
          if (dest > 0) { depth--; } // ]
        }
        // calculate jump address and replace (0x00 0x00)
        temp = dest + fixip;
        fixip = write(-fixip - 2, 2, new int[] {temp & 0x00FF, (temp & 0xFF00) >> 8});
      }
    }

    // (footer)
    // INT 20h                          CD 20
    ip = write(ip, 2, new int[] {0xCD, 0x20});

    // open output file
    try {
      out = new DataOutputStream(new FileOutputStream(f2));
    } catch (IOException e) {
      errout("Error opening '" + f2 + "' for output");
    }

    // write output buffer to disk
    for (n = 0; n < ip - START; n++) {
      try {
        out.write(buffer[n]);
      } catch (IOException e) {
        errout("Error writing to '" + f2 + "'");
      }
    }

    // close output file
    try {
      out.close();
    } catch (IOException e) {
      errout("Error closing '" + f2 + "'");
    }

    System.out.println("\r\nDone.");
  }

  //------------------------------------------------------------------
  // errout -- prints error message then terminates program execution
  //------------------------------------------------------------------
  public static void errout(String e) {
    System.out.print("\r\n" + e + ".");
    System.exit(1);
  }

  //------------------------------------------------------------------
  // usage -- prints usage information
  //------------------------------------------------------------------
  public static void usage() {
    System.out.println("Usage: java bfc [-o outfile] file[.bf] [-?]");
    System.out.println("Where: -o outfile   Specify output filename, default: file.com");
    System.out.println("       file         Input filename");
    System.out.println("       -?           Display usage information");
  }

  //------------------------------------------------------------------
  // write -- write code to code output buffer
  //------------------------------------------------------------------
  public static int write(int position, int num, int[] bytes) {
    for (int n = 0; n < num; n++) {
      buffer[position - START + n] = bytes[n];
    }
    return position + num;
  }

}
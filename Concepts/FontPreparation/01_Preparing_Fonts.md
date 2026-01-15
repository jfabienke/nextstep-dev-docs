# Preparing Fonts

NEXTSTEP uses PostScript fonts. These fonts must be prepared before they can be used. This section describes how to prepare a font for use with NEXTSTEP; it doesn't explain how to create a new font. Instructions for creating PostScript fonts can be found in the books The PostScript Language Reference Manual, The PostScript Language Tutorial and Cookbook, Adobe Type 1 Font Format, and PostScript Language Program Design, all by Adobe Systems, Incorporated.

---

## Components of a Font

All fonts must have at least these two components:

1. **An outline description file** — PostScript fonts contain instructions for constructing the outline of each character in the font; these instructions are contained in an outline description file. By convention, the file's name consists of the font's family name, a hyphen, then the remainder of the font's full name (sans whitespace). For example, the outline file for the Helvetica Bold Oblique font is `Helvetica-BoldOblique`.

2. **Adobe Font Metric (AFM) file** — An AFM file is an ASCII file that provides information such as the font's name, weight, and character bounding box (the complete format of an AFM file is given in the Adobe Font Metric Files specification). An AFM file is given the same name as its corresponding outline file, plus the extension `.afm`.

A font can also have a third component: **A set of bitmaps** that are used to display smaller font sizes on the screen. Although outline fonts at larger point sizes (24 points or larger) provide splendid screen images, at smaller sizes they can look horribly twisted. To amend this disfigurement, a font may sport size-specific bitmaps. (The fonts provided by NEXTSTEP typically have bitmaps for 10, 12, 14, 18, and 24 point sizes.)

Bitmap font files are typically named with the font size and a period followed by a contracted form of the font name, including weight and italic indications. For example, the Helvetica Bold Oblique font bitmaps are named `9.HelveBolObl`, `10.HelveBolObl`, and so on.

To see these three components of an existing font, open the directory `/NextLibrary/Fonts`. Each font appears in its own subdirectory named `fontName.font` (for example, `Helvetica-Bold.font`). In this subdirectory are the outline, AFM, and bitmap files for the font.

> **Important:** To work with NEXTSTEP, the fonts you construct must encode the space character at position 32 (decimal) in the outline file. The space character must also be represented in the AFM file and in any bitmap files you provide.

---

## Preparing a New Font

The first step in creating a font to use with NEXTSTEP is to create (or otherwise obtain) an outline font file. Given an outline font, you must create an AFM file.

### Creating the AFM File

Currently, there are no programming tools in NEXTSTEP designed specifically to help you create AFM files; they must be built by hand.

The Application Kit uses information in the AFM file when it displays commands in the Font menu and items in the Font panel. The Font panel's first column displays the font's family name as found in the AFM file. The font's full name, minus the family name, is displayed in the Font panel's second column.

The AFM file's weight entry provides the information the Application Kit needs for the operation of the Font menu's Lighter and Heavier commands. When a user chooses the Lighter or Heavier command, the Application Kit determines which new font to use by comparing the weight entry in the AFM file against this list (arranged from lightest to heaviest):

1. Ultra Light
2. Thin
3. Light, Extra Light
4. Book
5. Regular, Plain
6. Roman, Medium
7. Demi, Demi Bold
8. Semi Bold
9. Bold
10. Extra Bold
11. Heavy, Heavyface
12. Black
13. Ultra, UltraBlack, Fat
14. Extra Black, Obese

Starting at the row containing the current font's weight, the list is traversed upward (for the Lighter command) or downward (for the Heavier command) until a font name can be constructed that corresponds to one of the fonts available on the system. For example, if the user chooses Lighter and the current font is Times Bold, the Application Kit would check for Times Semi Bold, Times Demi, and Times Demi Bold (which aren't included in the standard release) before finding Times Roman (which is). If no font is found, no change is made.

### Creating the Bitmap Files

Bitmap files are in Adobe System's Character Bitmap Distribution Format (CBDF). See the Character Bitmap Distribution Format document for details.

> **Warning:** The global header for each CBDF file contains a SIZE entry that lists the point size of the characters and the x and y resolution of the device that will display the characters. For fonts intended for use with NEXTSTEP, the x and y resolution must be set to 75.

Once the bitmap files are finished, you're ready to create the "master" bitmap font file. The UNIX utility `prebuild` takes a set of CBDF files and builds them into a single bitmap font file. (For more information, see the UNIX manual page for `prebuild`.) The bitmap font file that `prebuild` creates has the name of the font plus the extension `.bepf`.

You also need to build screen-font metrics files corresponding to the bitmap fonts you have created. There's another UNIX utility called `screenafm` that takes the AFM file for your font and the CBDF files you created and generates AFM files for each size of the screen fonts. (For more information, see the UNIX manual page for `screenafm`.) These are used by the Application Kit to provide a more legible screen representation of your font when possible. (See the discussion of bitmap fonts in Section 7.4 of the PostScript Language Reference Manual, second edition.)

If you are providing a bitmap font to go with an outline font, check the outline font (not the AFM file) to be sure that it has a FontName entry in its top-level dictionary and that this entry is correctly set to the name of the font. (See section 5.3 of the PostScript Language Reference Manual for a description of font dictionaries.) The Window Server determines if a bitmap font is available for a particular outline font by searching for the bitmap font by name. It constructs the name by adding a `.bepf` extension to the name it finds in the font dictionary's FontName entry. If you're not providing bitmap fonts, the FontName entry isn't used.

---

## Installing the Font

Fonts are installed in three places in the file system:

1. **`/NextLibrary/Fonts`** — This directory contains fonts that are provided by NEXTSTEP. You never install your own fonts here.
2. **`/LocalLibrary/Fonts`** — This is where to put fonts that are intended for use by everyone at your site.
3. **`~/Library/Fonts`** — This is where fonts intended for your own use should go. They won't be accessible to others.

After you've determined where to store the font, create a subdirectory called `fontName.font`, and copy the font's files into the subdirectory.

If you install your font in `/LocalLibrary/Fonts` you must notify the system of the change. To do this you issue the `buildafmdir` command as root in a Terminal window. For example:

```bash
MyPrompt> su
Password: enter your password
HostName:1# buildafmdir ~/Library/Fonts
```

See the UNIX manual page for more information about `buildafmdir`.

---

## An Example: The Purissma Font

Here's an example of putting together a font family called Purissma. Purissma is a font that comes in two faces: Medium and Bold. We create a directory called `PurisFont`, and put in it the source files for the font:

```
Purissma    (the outline font for the medium weight)
Purissma-Bold    (the outline font for the bold weight)
Purissma.afm    (the AFM files)
Purissma-Bold.afm
10.Puris    (the CBDF files for Purissma)
12.Puris
14.Puris
18.Puris
24.Puris
10.PurisBol    (the CBDF files for Purissma-Bold)
12.PurisBol
14.PurisBol
18.PurisBol
24.PurisBol
```

### The Makefile

Next, we create a makefile for the directory. (See the UNIX manual page for `make`, for more information on makefiles.)

```makefile
#
# Makefile for Purissma font
#
# DSTROOT specifies where to install font.
DSTROOT = 'echo $HOME'/Library
#
# Rule for building bitmap font files
#
# CBDF Files for medium weight
PURISSMA_SOURCES = 10.Puris 12.Puris 14.Puris \
18.Puris 24.Puris
#
# CBDF Files for Bold weight
PURISSMABOLD_SOURCES = 10.PurisBol 12.PurisBol \
14.PurisBol 18.PurisBol 24.PurisBol

Purissma : Purissma.bepf Purissma-Bold.bepf \
Screen-Purissma.10.afm \
Screen-Purissma-Bold.10.afm

Purissma.bepf : $(PURISSMA_SOURCES)
	prebuild Purissma 3 90 Purissma $(PURISSMA_SOURCES)

Purissma-Bold.bepf : $(PURISSMABOLD_SOURCES)
	prebuild Purissma-Bold 3 91 Purissma \
	$(PURISSMABOLD_SOURCES)

Screen-Purissma.10.afm : $(PURISSMA_SOURCES)
	screenafm Purissma $(PURISSMA_SOURCES)

Screen-Purissma-Bold.10.afm : $(PURISSMABOLD_SOURCES)
	screenafm Purissma-Bold $(PURISSMABOLD_SOURCES)

# install rules

install : Purissma.bepf Purissma-Bold.bepf
	mkdirs $(DSTROOT)/Fonts/Purissma.font
	mkdirs $(DSTROOT)/Fonts/Purissma-Bold.font
	cp Purissma Purissma.* Screen-Purissma.* \
	$(DSTROOT)/Fonts/Purissma.font
	cp Purissma-Bold Purissma-Bold.* Screen-Purissma-Bold.* \
	$(DSTROOT)/Fonts/Purissma-Bold.font
	buildafmdir $(DSTROOT)/Fonts
```

### Understanding the Makefile

The `DSTROOT` definition establishes where this font is to be installed. We can reset this to `/LocalLibrary` if we want this font to be shared. Then, we define macros for the two sets of CBDF files. The first make rule is the one for Purissma, which simply depends on the bitmap font files. Then, we show how to build the bitmap font files.

The `prebuild` utility accepts the font name, and two numeric arguments: the font type and unique font identifier. In the current scheme, these are actually ignored. However, they are intended to identify this font through its font type (use font type 3 for user-defined fonts) and its unique ID (use any number in the range from 0 to 32767). The next argument is the character set name. A character set is a mapping of character names to positions in the prebuilt file. There is a standard character set, `ISOLatin1CharacterSet`, which the NeXT-provided fonts come in. If your prebuilt files contain exactly the 229 characters of the `ISOLatin1CharacterSet`, and the names you used are exactly those defined in that encoding, then you can use that as your character set name. However, if you aren't sure about how your CBDF files were created, the safest thing is to use the name of the font family as the character set name. The trailing arguments to the `prebuild` program are the names of the CBDF files to use. These must be in order from smallest to largest.

The final line of the makefile executes the `buildafmdir` command. `buildafmdir` alerts the Application Kit that the fonts in the specified directory have changed so that Font panels can be updated to show the new font.

### Building and Installing

Given the above Makefile, issuing the command:

```bash
make Purissma
```

builds the bitmap font files and the screen font metric files, and

```bash
make install
```

installs the font in `~/Library`. When the make command finishes, the fonts in the new font family will be available to applications that are launched subsequently and will appear in their Font panels.

---

*Release 3.3 Copyright ©1995 NeXT Computer, Inc. All Rights Reserved.*

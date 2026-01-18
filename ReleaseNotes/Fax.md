# 3.3 Release Notes: Fax

**Release 3.3** — Copyright ©1995 by NeXT Computer, Inc. All Rights Reserved.

There are no new fax features or bug fixes in Release 3.3 or 3.2. This file contains release notes for the 3.1 and 3.0 releases of Fax software. Items specific to the 3.1 release are listed first, followed by the Release 3.0 notes.

---

## Notes Specific to Release 3.1

### Bugs Fixed in Release 3.1

These bugs have been fixed in Release 3.1:

| Reference | Problem |
|-----------|---------|
| 29634 | **If a bad fax file is created by software, autoprinting won't print subsequent faxes.** |
| | **Description:** Once the bad fax is in the queue, each time a new fax comes in, autoprinting tries to print the bad fax. As a result, subsequent faxes aren't printed. In Release 3.1, if software creates a bad fax, autoprinting skips it after attempting to print it the first time. In addition, a mail message is generated to notify privileged users of the bad fax. |
| 29645 | **Fax List unchanged after deleting a fax.** |
| | **Description:** This bug results in the faxreader being able to list faxes etc, but not receiving a refresh messages after deleting a fax. The cause is duplicate connections on the same reply port, due to netInfo hangs. In Release 3.1, Faxxess deletes the old connection before creating a new one. |
| 30615 | **faxcleanup errors prevent reception of faxes** |
| | **Description:** This problem occurs with third party NXFax drivers, not with the HSD driver. |
| 31009 | **If you enable autoprinting and then disable it, deleted faxes never get removed from disk.** |
| | **Description:** After disabling autoprinting, fax files aren't being deleted from disk by delete because of hidden links. In Release 3.1, disabling autoprinting releases the hidden links, which allows the files to be deleted. |
| 31065 | **Apps crash when Fax is invoked** |
| | **Description:** This occurs if the font in the cover sheet isn't available. In Release 3.1, the fax printing mechanism checks for font availability and uses the default userfont if any specified fonts aren't available. |
| 31313 | **Subsequent faxes aren't autoprinted after printer runs out of paper.** |
| | **Description:** If a local printer runs out of paper while autoprinting a fax, the interrupted job finishes when the paper tray is refilled. However, faxes received during the interruption won't print automatically. In Release 3.1, subsequent faxes continue autoprinting as expected after paper is reloaded. |

---

## Notes Specific to Release 3.0

These notes were included with the Release 3.0 version of the Fax software.

### Gotchas

Watch out for the following:

- 2.x FaxReader is not compatible with 3.0 libNeXT; 3.0 FaxReader is not compatible with 2.x libNeXT.
- 3.0 Fax software spools faxes in format that 2.x software can't read (Group 4, which is 1/2 the size of Group 3 1D). FaxReader has an option for saving a fax in format that 2.0 FaxReaders can read (2x blowup in file size); this option is the G3 option in the Save As panel.
- 3.0 Fax software mails a message to every unrestricted user when the disk upon which incoming faxes are spooled approaches full. A bug in the pre-release software causes potentially hundreds of these messages to be mailed. To stop the messages, free up space on your disk by deleted unneeded files.

### Changes

The following additional changes have been made to Fax since Release 2.1:

- The Fax Send panel reads Draw documents (`/NextDeveloper/Demos/Draw.app`), saved with Form Entries, instead of rtf files for cover sheets when faxing. Use the Fax Cover Sheet menu in the Draw tools menu to add form entries. Any of the other Draw tools can be used to produce background graphics.
- The fax software now returns both success and failure messages either by mail (when the Notify box is clicked) or by alert. The alerts now show the Fax icon, instead of the Printer icon. Whereas previously, no more than on alert was permitted on the screen (the subsequent ones were ignored), now all fax alerts appear on the screen until you dismiss them.
- The fax software supports modems that can send/receive 2D as well as 1D data.
- The fax software spools faxes in G4 format, reducing storage requirements by typically 50%.
- The FaxReader is greatly changed. Faxes are paginated correctly, using the printable area of the page. The user can choose to trim oversize pages to the printable area of the page, or break each oversize page into printable pieces or scale oversized pages to fit. Faxes can be rotated thru 90,180 and 270 degrees. Controls exist for page forward and backward as well as jumping to page N (similar to WriteNow scroll bar). Faxes can be shared across the net for reading faxes as well as writing faxes.
- The modem driver deals with received faxes, whose pages incorporate different parameters (eg first page is Standard, second page Fine resolution).
- The modem driver uses `cu[ab]`, which makes it immune to modem settings, etc, that cause DTR to drop.
- As the Class 2 spec has still not been approved and as various modems purporting to be Class 2 differ significantly, our driver is now only explicitly supports the HSD Fax modem.
- All fax messages are localizable.

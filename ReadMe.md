MotoLangDump
============

An utility to extract stock string translations from MotoMAGX and EZX phones.

## Usage (PC)

The first step is to unpack the strings from the entire firmware:

1. Disassemble the SBF firmware file to the SMG code groups using SBF Tool or SBF-Recalc programms.
2. Unpack or mount required SMG files to get a full rootfs image of the firmware.
3. Use the [Code Groups] article to understand which SMG will need to be unpacked or mounted.

	1. MotoEZX: CG43/CG44 (rootfs), CG42 (setup), CG41 (language), CG37 (secure_setup), CG34 (data_resource), CG35 (userfs_db), CG36 (userfs_general)
	2. MotoMAGX: CG36 (rootfs), CG37 (userfs), CG43 (setup), CG44 (secure_setup), CG46 (language), CG52 (data_resource)

4. Use various Linux utils: uncramfs, unsquashfs, and the "[jffs2_mount_mtdram.sh](util/jffs2_mount_mtdram.sh)" script for unpacking or mounting file systems.
5. Use the following command to collect using strings from the firmware:

	```bash
	find fw_rootfs/ -type f -exec strings {} \; | grep -e "^TXT" | uniq | sed -e "s/,.*$//" > Strings.txt
	```

## Usage (Phone)

1. Put the "MotoLangDump" and "Strings.txt" files to phone, e.g. to your memory card (`/mmc/mmca1/`).
2. Load the environment variables to run the program.

	```sh
	. /etc/initservices/services/ezxenv.sh   # MotoMAGX
	. /home/native/.profile                  # MotoEZX
	```

	*Note:* It is not necessary on most custom firmware mods.

3. Use the following command to unpack strings from "Strings.txt":

	```sh
	./MotoLangDump Strings.txt
	```

4. Take the "Strings_Table.txt" file from the current directory.

*Note:* The language of the translated strings in the table will depend on the installed language in the firmware and the selected language in the phone settings.

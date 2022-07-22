/*
 * About:
 *   The "MotoLangDump" utility designed to extract stock string translations from MotoMAGX and EZX phones.
 *
 * Author:
 *   EXL
 *
 * License:
 *   MIT
 *
 * History:
 *   23-Jul-2022: Public version.
 *   21-Jul-2022: Use UTF-8 codec for QTextStream.
 *   20-Jul-2022: First draft version.
 *
 * Host commmands:
 *   $ find fw_rootfs/ -type f -exec strings {} \; | grep -e "^TXT" | sort | uniq | sed -e "s/,.*$//" > Strings.txt
 *
 * Phone commmands:
 *   $ . /etc/initservices/services/ezxenv.sh   # MotoMAGX
 *   $ . /home/native/.profile                  # MotoEZX
 *
 *   $ cd /mmc/mmca1/
 *   $ ./MotoLangDump Strings.txt
 */

#include <ZApplication.h>

#include <qfile.h>
#include <qfileinfo.h>
#include <qobject.h>
#include <qtextstream.h>

#include <cstdio>

#define IDX_STRING_MAX_LENGTH  6
#define KEY_STRING_MAX_LENGTH  128
#define IN_FILENAME            "Strings.txt"
#define OUT_FILENAME           "Strings_Table.txt"

QString arrange_string(const QString &str, bool digit) {
	QString spaces;
	spaces.fill(' ', ((digit) ? IDX_STRING_MAX_LENGTH : KEY_STRING_MAX_LENGTH)- str.length());
	return (digit) ? spaces + str : str + spaces;
}

int main(int argc, char *argv[]) {
	/* Nothing will work without this line! */
	ZApplication application(argc, argv);

	const char *exe_filename = argv[0];
	if (argc != 2) {
		fprintf(stderr, "Usage:\n\t%s " IN_FILENAME "\n", exe_filename);
		return 1;
	}

	const char *in_filename = argv[1];
	QFile in_file(in_filename);
	if (!in_file.open(IO_ReadOnly)) {
		fprintf(stderr, "Cannot open '%s' file for read.\n", in_filename);
		return 2;
	}

	QFile out_file(QString("%1/%2").arg(QFileInfo(exe_filename).dirPath(true)).arg(OUT_FILENAME));
	if (!out_file.open(IO_WriteOnly)) {
		fprintf(stderr, "Cannot open '%s' file for write.\n", OUT_FILENAME);
		return 3;
	}

	QTextStream in_text_stream(&in_file);
	QTextStream out_text_stream(&out_file);
	out_text_stream.setEncoding(QTextStream::UnicodeUTF8);
	for (int i = 1, j = 0; !in_text_stream.atEnd(); ++i) {
		const QString key = in_text_stream.readLine();
		QString value = QObject::tr(key.data(), key);
		QString index = arrange_string(QString("%1").arg(i), true);
		if (key == value) {
			fprintf(stdout, "=> %s ERROR: '%s'\n", index.data(), key.data());
			value = "ERROR: THERE IS NO TRANSLATION FOR THIS STRING!";
		} else {
			fprintf(stdout, "=> %s    OK: '%s'\n", index.data(), key.data());
			++j;
			out_text_stream
				<< arrange_string(QString("%1").arg(j), true) + "   |||   "
				<< arrange_string(key, false) + "   |||   "
				<< value + "\n";
		}
	}
	out_file.close();
	in_file.close();

	return 0;
}

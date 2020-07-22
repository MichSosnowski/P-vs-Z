/*  Policjanci vs złodziej, the static game 2D
	Copyright(C) 2020 Michał Sosnowski

	This program is free software : you can redistribute it and /or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation version 3 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include "glowne.h"
#include <windows.h>

#define ATRYBUT 0   //argument przyjomwany przez funkcje CreateMutexA()

int main(int argc, char* argv[]) {
	//sprawdzanie, czy program jest już otworzony
	CreateMutexA(ATRYBUT, FALSE, "Local\\PvsZ");
	if (GetLastError() == ERROR_ALREADY_EXISTS) exit(EXIT_SUCCESS);

	QApplication app(argc, argv);

	Glowne okno;   //tworzy główne okno programu

	return app.exec();
}

/*
 * =====================================================================================
 *
 *       Filename:  toolbox.c
 *
 *    Description:  toolbox - Sammlung für ncurses-Spiele
 *    				Dieser Quelltext versucht die Fähigkeiten von C auszuschöpfen, daher
 *    				ist C99 oder neuer notwendig, um ihn zu kompilieren.
 *
 *        Version:  0.003
 *    letzte Beta:  0.000
 *        Created:  12.09.2011 11:52:00
 *          Ended:  00.00.0000 00:00:00
 *       Revision:  none
 *       Compiler:  clang
 *        compile:  clang -o toolbox toolbox.c -lncurses
 *
 *         Author:  Sascha K. Biermanns (saschakb), saschakb@gmail.com
 *        Company:
 *        License:  ISC
 *
 *   Copyright (C)  2011, Sascha K. Biermanns
 *
 * ====================================================================================
 *
 *   Permission to use, copy, modify, and/or distribute this software for any
 *   purpose with or without fee is hereby granted, provided that the above
 *   copyright notice and this permission notice appear in all copies.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * =====================================================================================
 *
 *   Letze Änderungen:
 *   - 12.09.2011 Beginn an der Arbeit des Moduls
 *                System zum einfachen Ändern der Farben eingeführt
 *                textausgabe mit variabler Parameterliste ausgestattet
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Zufallsgenerator
#include <string.h>
#include <ncurses.h> // Farbige Grafische Ausgabe
#include <locale.h>
#include <stdarg.h> // Für die VA-Liste
#include "toolbox.h" // Die Modul-Beschreibung

static int vfarbe = weiss; // Vordergrundfarbe
static int hfarbe = schwarz; // Hintergrundfarbe

// Funktion: Hintergrundfarbe ändern
void hintergrundfarbe(enum farben hf) {
  hfarbe = hf;
  color_set((8 * vfarbe) + hfarbe + 1, 0);
}

// Implementation: Ja-Nein-Frage
bool janeinfrage(char *frage) {
	char eingabe;
	vordergrundfarbe(zyan);	textausgabe(frage);
	eingabe = taste();
	vordergrundfarbe(weiss);
	if((eingabe == 'j') || (eingabe == 'J'))
		return true;
	else
		return false;
}

// Initialisierung der ncurses-Umgebung
void ncurses_init(void (*funktion)()) {
	// Umgebungsvariable setzen
	setlocale(LC_ALL, "");

	initscr(); // beginne ncurses
	keypad(stdscr, true); // Keymapping aktivieren
	cbreak(); // kein Warten bei der Eingabe auf ENTER
	echo(); // Cursort-Echo
	scrollok(stdscr, true); // Automatisches Scrollen aktivieren
	start_color(); // Beginne mit Farben
        // Initialisierung aller Farbpaare
        for(int x = schwarz; x <= weiss; x++)
          for(int y = schwarz; y <= weiss; y++)
            init_pair((8 * x) + y + 1, x, y);
        vordergrundfarbe(weiss);
        hintergrundfarbe(schwarz);
	clear(); // Bildschirm löschen
	curs_set(0);
	atexit(funktion); // Routine, die bei der Beendung ausgeführt wird
}

// Implementation: Taste
char taste(void) {
	int puffergroesse;
	char zeichen;

	noecho();
	zeichen = getch();
	echo();
	return(zeichen);
}


// Implementation: Textausgabe
void textausgabe(char *gesamttext, ...) {
	int zeilenlaenge = COLS; // COLS ist eine ncurses Variable
	int maxzeilen = LINES; // LINES ist eine ncurses Variable

	char textzeile[zeilenlaenge]; // Ausgabezeile
	int i; // Schleifenzähler
	int j; // Schleifenzähler
	int zeile = 0;
	bool erstausgabe = true;
	int x, y; // Speichern die Bildschirmkoordinaten (für getyx)

    // Reservierung für den maximalen Speicherplatz, den resttext benötigt
    // -------------------------------------------------------------------
    char *sicherheitszeiger = (char*) malloc( (sizeof(gesamttext) + 100) * sizeof(char));
    if(!sicherheitszeiger) {
        vordergrundfarbe(rot);
        printw("Fehler!\nsicherheitszeiger in textausgabe() erhielt keinen Speicher!\n");
        vordergrundfarbe(weiss);
        exit(EXIT_FAILURE);
    }
    char *resttext = sicherheitszeiger;
    // -------------------------------------------------------------------
    
    // Verarbeitung der Parameterliste und Umwandlung der Parameter + Text zu einem String
    // -----------------------------------------------------------------------------------
    va_list par; // Parameterliste
    va_start(par, gesamttext);
    vsprintf(resttext, gesamttext, par);
    va_end(par);
    // -----------------------------------------------------------------------------------

	for(i = 0; i < zeilenlaenge; i++)
		textzeile[i] = '\0'; // Sicherheitslöschung, sonst gibt es Fehler bei der Leerzeilenausgabe
	while(strlen(resttext) > (zeilenlaenge - 1)) {
		for(i = (zeilenlaenge - 1); (*(resttext+i) != ' ') && (i > 0); i--);
		if(!i)
			i = (zeilenlaenge - 1); // Das Wort ist so länger als die verdammte Zeile
		for(j = 0; (*(resttext+j) != '\n') && (j < i); j++);
		if(j < i)
			i = j; // Auf das Zeilenendezeichen verkürzen
		strncpy(textzeile, resttext, i); // Den Textteil kopieren
		resttext += i+1;
		while(*resttext == ' ')
			resttext++;
		// Prüfen, ob wir in der vorletzten Zeile angekommen sind
	  	getyx(stdscr, y, x); // Cursorposition feststellen
		if((erstausgabe == true) && (y >= (maxzeilen - 1))) {
			weiter();
			zeile = 0;
			erstausgabe = false;
		}
		if(zeile == (maxzeilen - 1)) {
				weiter();
				zeile = 0;
		}
		printw("%s\n", textzeile);
		for(i = 0; i < zeilenlaenge; i++)
			textzeile[i] = '\0'; // Sicherheitslöschung, sonst gibt es Fehler bei der Leerzeilenausgabe
		zeile += 1;
	}
	// Text ist kürzer als eine Zeile.
	// Prüfen, ob wir in der vorletzten Zeile angekommen sind
	if(zeile == (maxzeilen - 1)) {
			weiter();
			zeile = 0;
	}
	printw("%s\n", resttext);
	refresh();
    // Sicherheitszeiger löschen, sonst gibt es üble Speicherlöcher ;)
    if(!sicherheitszeiger)
        free(sicherheitszeiger);
}

// Implementation: Texteingabe
void texteingabe(char *text, char *eingabe, unsigned int laenge) {
  textausgabe(text);
  attrset(A_BOLD);
  getnstr(eingabe, laenge);
  attrset(A_NORMAL);
  // return eingabe;
}

// Funktion: Vordergrundfarbe ändern
void vordergrundfarbe(enum farben vf) {
  vfarbe = vf;
  color_set((8 * vfarbe) + hfarbe + 1, 0);
}

// Implementation: waehle
int waehle(char* beschreibung, int maxzahl) {
	int ergebnis;
	char eingabe[20];

        while((ergebnis < 1) || (ergebnis > maxzahl)) {
                texteingabe(beschreibung, eingabe, 20);  
		ergebnis = atoi(eingabe);
	}
	return ergebnis;
}

// Implementation: Weiter
void weiter(void) {
    attrset(A_BLINK);
	printw("--- Bitte ENTER-Taste drücken um fortzufahren ---");
    attrset(A_NORMAL);
	getch();
	clear(); // Bildschirm löschen
	curs_set(0);
}


// Implementation x-seitiger Würfel
int wuerfel(unsigned int maximalzahl) {
	// aus z.B. 0..5 wird 1..6
	return (rand() % (maximalzahl - 1)) + 1;
}

// Initialisierung der Zufallszahlen
void zufall_per_zeit(void) {
  time_t jetzt;
  jetzt = time((time_t *) NULL);
  srand((unsigned) jetzt);
}

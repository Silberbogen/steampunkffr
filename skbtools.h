/*
 * =====================================================================================
 *
 *       Filename:  skbtools.h
 *
 *    Description:  saschakb toolbox - Sammlung für ncurses-gestützte Anwendungen
 *    				Dieser Quelltext versucht die Fähigkeiten von C auszuschöpfen, daher
 *    				ist C99 oder neuer notwendig, um ihn zu kompilieren.
 *
 *        Version:  0.001
 *    letzte Beta:  0.000
 *        Created:  14.09.2011 11:40:00
 *          Ended:  00.00.0000 00:00:00
 *       Revision:  none
 *       Compiler:  c99/c11
 *        compile:  make
 *
 *         Author:  Sascha K. Biermanns (skbierm), skbierm@gmail.com
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
 *   - 14.09.2011 Beginn an der Arbeit des Moduls
 *                Es wurden die Prototypen
 *                - nstring()
 *                - nstrlen()
 *                - nstrlencorr
 *                - nstrcorr()
 *                neu aufgenommen
 *   - 14.11.2012 Vereinfachung bei Strukturnamen, kleine Korrekturen
 *
 * =====================================================================================
 */

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Zufallsgenerator
#include <string.h>
#include <ncurses.h> // Farbige Grafische Ausgabe
#include <locale.h>
#include <stdarg.h> // Für die VA-Liste
*/

#ifndef _SKBTOOLS_H_
#define _SKBTOOLS_H_

#include <ncurses.h>

// Makro zur Erklärung statischer Variablen
#define Staticdef( typ, variable, initwert) \
	static typ variable = 0; \
	if ( !(var) ) var = (initwert);

// ----------------------------------------------------------
// farben - kann ich mir leichter als die Originale merken ;)
// ----------------------------------------------------------

enum farben { schwarz = COLOR_BLACK,
              rot = COLOR_RED,
              gruen = COLOR_GREEN,
              gelb = COLOR_YELLOW,
              blau = COLOR_BLUE,
              magenta = COLOR_MAGENTA,
              zyan = COLOR_CYAN,
              weiss = COLOR_WHITE };

// -----------------------------
// Neuer Datentyp nstr_s (nstring)
// -----------------------------
typedef struct nstr_s {
	unsigned int len;  // Die Länge der Zeichenkette, inklusive des \0
	char *str; // Zeiger auf die Zeichenkette
} nstr_s;

// ------------------------------------
// Neuer Datentyp narr_s = Array von nstr_s
// ------------------------------------

typedef struct narr_s {
	unsigned int cnt; // Die Größe des Feldes
    nstr_s **elm; // Zeiger auf das Feld
} narr_s;

// ----------------------------------
// Prototypen für den ncurses-Bereich
// ----------------------------------

// Funktion: Beenden des Programms
// Implementation: Gibt eine farbige Meldung text in der Farbe f aus und beendet das Programm mit dem Status status
void beenden(enum farben f, int status, char * text, ...);

// Funktion: Hintergrundfarbe
// Implementation: Setzt die Hintergrundfarbe auf den Farbwert f
void hintergrundfarbe(enum farben f);

// Funktion: Hinweis
// Implementation: Gibt eine Meldung text in der Farbe f aus und wartet anschließend auf einen Tastendruck.
// Hinweis: Die Vordergrundfarbe des Textes ist nach Rückkehr aus der Funktion Weiss
void hinweis(enum farben f, char * text, ...);

// Funktion: Ja/Nein-Frage
// Implementation: Gibt den Text text auf dem Bildschirm aus und wartet auf die Antwort
// Hinweis: Man muß ausschließlich 'j' oder 'J' tippen zum bestätigen, sonst ist die Antwort NEIN.
// Rückgabe: Bei 'j' oder 'J': wahr
//           ansonsten:        falsch
bool janeinfrage(char * text); // Funktion: Ja-Nein-Frage

// Funktion: ncurses_initialisierung
// Implementation: Initialisiert ncurses auf die von mir üblicherweise gewünschte Art.
void ncurses_init(void (*)());

// Funktion: Taste
// Implementation: Schaltet die Echo-Funktion aus und wartet auf einen Tastendruck, anschließend wird das Echo wieder eingeschaltet
// Rückgabe: Die gedrückte Taste
char taste(void);

// Funktion: Textausgabe
// Implementation: Erlaubt die Ausgabe eines Textes auf dem Bildschirm. Die Ausgabe erfolgt in der von Vordergrundfarbe und Hintergrundfarbe ausgewählten Farbwahl. Die Ausgabe kann nach Art des printf-Mechanismus formatiert werden.
// Hinweis: Die Ausgabe wird Zeile für Zeile ausgegeben. Ist die Ausgabe länger als die Bilschirmgröße, so wird zu einem Tastendruck augefordert, erst danach wird weiterer Text ausgegeben. Nach Beendigung der Ausgabe befindet sich der Cursor am Beginn der darauffolgenden Zeile.
void textausgabe(char *, ...);

// Funktion: Texteingabe
// Implementation: Erlaubt das eingeben einer Zeichenkette text mit einer vorgegebenen Länge.
// Hinweis: Die Länge kann NICHT überschritten werden. Die Eingabe erfolgt bei eingeschaltetem FETT-Attribut des Textes.
void texteingabe(char * text, unsigned int laenge);

// Funktion: Vordergrundfarbe
// Implementation: Setzt die Vordergrundfarbe grundfarbe auf den Farbwert f
void vordergrundfarbe(enum farben f);

// Funktion: Wähle
// Implementation: Die Funktion gibt den Text text aus und erlaubt die Eingabe einer numerischen Auswahl zwischen 1 und der Obergrenze.
// Implementation: Die Eingabe wird wiederholt, bis das eingegebene Ergebnis im Bereich zwischen 1 und der Obergrenze liegt.
int waehle(char * text, int obergrenze);

// Funktion: Weiter
// Implementation: Die Funktion gibt einen Text aus, der anzeigt, das eine beliebige Taste zum Fortfahren gedrückt werden soll.
// Hinweis: Nach dem Drücken der Taste wird der Bildschirm gelöscht und der Cursor in die linke obere Ecke gesetzt.
void weiter(void);

// -------------------------------------
// Prototypen für den Mathematik-Bereich
// -------------------------------------

// Funktion: Würfel
// Implementatino: Ahmt einen Würfel mit seitenanzahl Seiten nach 
// Rückgabe: Eine zufällig gewürfelte Zahl zwischen 1 und seitenanzahl
int wuerfel(unsigned int seitenanzahl);

// Funktion: Zufall per Zeit
// Implementation: Initialisiert den Zufallszahlengenerator anhand der aktuellen Uhrzeit
void zufall_per_zeit(void);

// -------------------------------
// Prototypen für den nstr_s-Bereich
// -------------------------------

// Funktion: nstring
// Implementation: Die Funktion gibt den reinen Zeichenketten-Teil des nstrings zurück
// Rückgabe: Zeichenkette auf den Zeichenketten-Teil des nstrings oder NULL
char * nstring(const nstr_s t);

// Funktion: nstring-Länge
// Implementation: Die Funktion gibt die Länge des nstrings zurück
// Rückgabe: Länge der nstring-Zeichenkette
int nstrlen(const nstr_s t);

// Funktion: nstring-Länge korrekt? - sonst Veränderung des Größenfelds
// Implementation: Überprüft die Länge des nstrings und korrigiert eventuell die gespeicherte Länge
// Rückgabe: wahr:   Die Länge stimmte überein
//           falsch: Die Länge stimmte nicht überein und wurde korrigiert.
bool nstrlencorr(nstr_s * t);

// Funktion: nstring-Länge korrekt? - sonst Veränderung der Zeichenkette
// Implementation: Überprüft die Länge des nstrings und korrigiert eventuell die Zeichenkette
// Hinweis: Es kann bei dieser Funktion zu einem Informationsverlust der Zeichenkette kommen.
// Rückgabe: wahr:   Die Länge stimmte überein
//           falsch: Die Länge stimmte nicht überein, der Fehler wurde korrigiert.
bool nstrcorr(nstr_s * t);

// Funktion: nstringnew
// Implementation: Diese Funktion erstellt einen Speicherbereich für einen neuen nstring - und fügt automatisch ein \0 zur Beendigung hinzu.
// Vorteil: Jeder nstring ist mit /0 abgeschlossen
// Rückgabe: Bei Erfolg: Zeiger auf die nstring-Struktur
//           ansonsten:  NULL
nstr_s * nstrnew(const char * zeichenkette);

// Funktion:  nstringdelete
// Implementation: Gibt den von der nstring-Struktur belegten Speicher wieder frei
// Rückgabe: wahr bei vollem Erfolg - falsch deutet an, das es zu einem teilweisen oder vollständigen Fehler kam
bool nstrdel(nstr_s * zeichenkette);

// Funktion:  nstringadd
// Implementation: Funktion, die weitere Zeichen an einen bereits vorhanden nstring anfügt
// Vorteil: Der nstring wird wieder korrekt mit /0 abgeschlossen
// Rückgabe: Zeiger auf den neuen Speicherort von nstring
nstr_s * nstradd(nstr_s * ziel,  const char * quelle);

// Funktion:  nstringcompare
// Implementation: Vergleich zweier gegebener nstrings
// Rückgabe: Negativ, wenn string1 <  string2 ist
//           Null,    wenn string1 == string2 ist
//           Positiv, wenn string1 >  string2 ist
int nstrcmp(const nstr_s * string1, const nstr_s * string2);

// Funktion: nstringcollate
// Implementation: Funktion, die 2 gegeben nstrings mittels LC_COLLATE vergleicht
// Rückgabe: Negativ, wenn string1 <  string2 ist
//           Null,    wenn string1 == string2 ist
//           Positiv, wenn string1 >  string2 ist
int nstrcoll(const nstr_s * string1, const nstr_s * string2);

// Funktion: nstringset
// Implementation: Funktion, die einen bereits existenten nstring mit neuen Zeichen überschreibt
// Rückgabe: Zeiger auf den neuen nstring
nstr_s * nstrset(nstr_s * ziel,  const char * quelle);

// Funktion: nstringfindfirstchar
// Implementation: Funktion, die das Auftreten des ersten Zeichens aus einer Liste ermittelt
// Rückgabe: Bei Erfolg: Zeiger auf das erste gefundene Zeichen
//           ansonsten:  NULL
char * nstrpbrk(nstr_s * ziel, const char * suchzeichenliste);

// Funktion: nstringfindlastchar
// Implementation: Funktion, die nach dem letzten Auftreten des Zeichens sucht
// Rückgabe: Bei Erfolg: Zeiger auf das letzte Vorkommen des Zeichens
//           ansonsten:  NULL
char * nstrrchr(nstr_s * ziel, const int suchzeichen);

// -------------------------------
// Prototypen für den narr_s-Bereich
// -------------------------------

// Funktion: nstringarraynew
// Implementation: Erstellt ein Feld (Array) von nstrings der Größe anzahl
// Rückgabe: Bei Erfolg: Zeiger auf die narr_s-Struktur
//           ansonsten: NULL
narr_s * narrnew(const unsigned int anzahl);

// Funktion: nstringarraydelete
// Implementation: Funktion, die ein bestehendes narr_s löscht
// Rückgabe: Bei Erfolg: wahr
//           ansonsten:  falsch
bool narrdel(narr_s * ziel);

// Funktion: nstringarrayadd
// Implementation: Funktion fügt einem bestehnden narr_s einen weitere anzahl nstrings hinzu und vergrößert damit das narr_s
// Rückgabe: Bei Erfolg: Zeiger auf die geänderte narr_s-Struktur
//           ansonsten:  NULL
narr_s * narradd(narr_s * ziel,  const unsigned int anzahl);

// Funktion: nstringarrayremove
// Implementation: Funktion, die das Element index aus dem narr_s entfernt und das narr_s verkleinert.
// Rückgabe: Bei Erfolg: wahr
//           ansonsten:  falsch
bool narrrmv(narr_s *ziel,  const unsigned int index);

#endif // _SKBTOOLS_H_


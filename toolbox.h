/*
 * =====================================================================================
 *
 *       Filename:  toolbox.h
 *
 *    Description:  toolbox - Sammlung für ncurses-Spiele
 *    				Dieser Quelltext versucht die Fähigkeiten von C auszuschöpfen, daher
 *    				ist C99 oder neuer notwendig, um ihn zu kompilieren.
 *
 *        Version:  0.004
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
 *   - 13.09.2011 texteingabe() Prototyp korrigiert
 *                beenden() Prototyp hinzugefügt
 *                hinweis() Prototyp hinzugefügt
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

#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include <ncurses.h>

enum farben { schwarz = COLOR_BLACK,
              rot = COLOR_RED,
              gruen = COLOR_GREEN,
              gelb = COLOR_YELLOW,
              blau = COLOR_BLUE,
              magenta = COLOR_MAGENTA,
              zyan = COLOR_CYAN,
              weiss = COLOR_WHITE };

void beenden(enum farben f, int status, char* text, ...); // Funktion: Farbige Beenden-Funktion

void hintergrundfarbe(enum farben); // Funktion: Hintergrundfarbe ändern

void hinweis(enum farben f, char* text, ...); // Funktion: Hinweis - für Fehlermeldungen oder ähnliches

bool janeinfrage(char *); // Funktion: Ja-Nein-Frage

void ncurses_init(void (*)()); // Initialisierung der ncurses-Umgebung

char taste(void); // Funktion: Taste

void textausgabe(char *, ...); // Funktion: Textausgabe

void texteingabe(char *, unsigned int); // Funktion: Texteingabe

void vordergrundfarbe(enum farben); // Funktion: Vordergrundfarbe ändern

int waehle(char*, int); // Funktion: waehle

void weiter(void); // Funktion: Weiter

int wuerfel(unsigned int); // Funktion: Wuerfel

void zufall_per_zeit(void); // Initialisierung der Zufallszahlen

#endif


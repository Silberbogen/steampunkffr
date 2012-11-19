/*
 * =====================================================================================
 *
 *       Filename:  steampunkffr.h
 *
 *    Description:  Steampunk FFR - Der Anfang
 *    				Ein "Das-ist-dein-Abenteuer"-Roman
 *    				Dieser Quelltext versucht die Fähigkeiten von C auszuschöpfen, daher
 *    				ist C99 oder neuer notwendig, um ihn zu kompilieren.
 *
 *        Version:  0.002
 *    letzte Beta:  0.000
 *        Created:  28.08.2012 02:05:00
 *          Ended:  00.00.0000 00:00:00
 *       Revision:  none
 *       Compiler:  c99/c11
 *        compile:  make
 *
 *         Author:  Sascha K. Biermanns (skbierm), skbierm@gmail.com
 *        Company:
 *        License:  ISC
 *
 *   Copyright (C)  2011-2012, Sascha K. Biermanns
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
 *   - 22.05.2011 Beginn an der Arbeit des Spiels
 *	 - 28.08.2012 steampunkffr.h hinzugefügt, teile aus der .c-Datei ausgelagert.
 *   - 14.11.2012 Vereinfachung bei Strukturnamen, kleine Korrekturen
 *
 * =====================================================================================
 */

#ifndef _STEAMPUNKFFR_H_
#define _STEAMPUNKFFR_H_

#include "skbtools.h"

// Makro zur Erklärung statischer Variablen, die im Anschluss daran mit
//  einem VARIABLEN Wert initialisiert werden können.
#define Staticdef( typ, variable, initwert) \
	static typ variable = 0; \
	if ( !(var) ) var = (initwert);

// Prototyp für Funktionszeiger
// Da er vom Typ void funktion(void) ist, kann er sämtliche Funktionen
// mit Namen ortXXX abbilden, wobei XXX für die Raumnummer steht.
typedef void funktion_s(void);

// Neuer Typ: charakter_s
// Zur Speicherung der Kampfwerte

typedef struct charakter_s {
	char name[30];
	int gewandheit;
	int gewandheit_start;
	int staerke;
	int staerke_start;
	int glueck;
	int glueck_start;
} charakter_s;

// -------------------
// Funktionsprototypen
// -------------------

// Funktion: Teste dein Glück
bool tdg(charakter_s *);

// Funktion: Kampfrunde
bool kampfrunde(charakter_s *, charakter_s *, void (*)());

// Funktion: Kampf
bool kampf(charakter_s *, charakter_s *, int, bool, void (*)());

// Funktion: Momentane Werte
void momentane_werte(charakter_s *);

// Funktion: Auswahl
void auswahl(char *, int, ...);

// Funktion: VersucheDeinGlueck
void versuchedeinglueck(void (*)(), void (*)());

// Funktion: VersucheDeineGewandheit
void versuchedeinegewandheit(void (*)(), void (*)());

// Funktion: Flucht
void flucht(void (*funktion1)());

// Funktion: Mahlzeit
void mahlzeit(void);

// Funktion: Gewandheitstrank trinken
void gewandheitstrank_trinken();

// Funktion: Stärketrank trinken
void staerketrank_trinken();

// Funktion: Glückstrank trinken
void glueckstrank_trinken();

// Funktion: Objkekt ablegen
void objekt_ablegen();

// Funktion: gewandheitssteigerung
void gewandheitssteigerung(int, int);

// Funktion: staerkesteigerung
void staerkesteigerung(int, int);

// Funktion: glueckssteigerung
void glueckssteigerung(int, int);

// Funktion: speichern
int speichern(void);

// Funktion: laden
int laden(void);

// Funktion: quit
void quit();

// Funktion: Rätsel
bool raetsel(char *raetseltext, char *antworttext);

// Funktion: Zweisamkeit
void zweisamkeit(int);

// ---------------------------------
// Deklaration der Spielroutinen
// ---------------------------------

void vorwort();
void intro();
void ort1();
void ort2();
void ort3();
void ort4();
void ort5();
void ort6();
void ort7();
void ort8();
void ort9();
void ort10();
void ort11();
void ort12();
void ort13();
void ort14();
void ort15();
void ort16();
void ort17();
void ort18();
void ort19();
void ort20();
void ort21();
void ort22();
void ort23();
void ort24();
void ort25();
void ort26();
void ort27();
void ort28();
void ort29();
void ort30();
void ort31();
void ort32();
void ort33();
void ort34();
void ort35();
void ort36();
void ort37();
void ort38();
void ort39();
void ort40();
void ort41();
void ort42();
void ort43();
void ort44();
void ort45();
void ort46();
void ort47();
void ort48();
void ort49();
void ort50();
void ort51();
void ort52();
void ort53();
void ort54();
void ort55();
void ort56();
void ort57();
void ort58();
void ort59();
void ort60();
void ort61();
void ort62();
void ort63();
void ort64();
void ort65();
void ort66();
void ort67();
void ort68();
void ort69();
void ort70();
void ort71();
void ort72();
void ort73();
void ort74();
void ort75();
void ort76();
void ort77();
void ort78();
void ort79();
void ort80();
void ort81();
void ort82();
void ort83();
void ort84();
void ort85();
void ort86();
void ort87();
void ort88();
void ort89();
void ort90();
void ort91();
void ort92();
void ort93();
void ort94();
void ort95();
void ort96();
void ort97();
void ort98();
void ort99();
void ort100();
void ort101();
void ort102();
void ort103();
void ort104();
void ort105();
void ort106();
void ort107();
void ort108();
void ort109();
void ort110();
void ort111();
void ort112();
void ort113();
void ort114();
void ort115();
void ort116();
void ort117();
void ort118();
void ort119();
void ort120();
void ort121();
void ort122();
void ort123();
void ort124();
void ort125();
void ort126();
void ort127();
void ort128();
void ort129();
void ort130();
void ort131();
void ort132();
void ort133();
void ort134();
void ort135();
void ort136();
void ort137();
void ort138();
void ort139();
void ort140();
void ort141();
void ort142();
void ort143();
void ort144();
void ort145();
void ort146();
void ort147();
void ort148();
void ort149();
void ort150();
void ort151();
void ort152();
void ort153();
void ort154();
void ort155();
void ort156();
void ort157();
void ort158();
void ort159();
void ort160();
void ort161();
void ort162();
void ort163();
void ort164();
void ort165();
void ort166();
void ort167();
void ort168();
void ort169();
void ort170();
void ort171();
void ort172();
void ort173();
void ort174();
void ort175();
void ort176();
void ort177();
void ort178();
void ort179();
void ort180();
void ort181();
void ort182();
void ort183();
void ort184();
void ort185();
void ort186();
void ort187();
void ort188();
void ort189();
void ort190();
void ort191();
void ort192();
void ort193();
void ort194();
void ort195();
void ort196();
void ort197();
void ort198();
void ort199();
void ort200();
void ort201();
void ort202();
void ort203();
void ort204();
void ort205();
void ort206();
void ort207();
void ort208();
void ort209();
void ort210();
void ort211();
void ort212();
void ort213();
void ort214();
void ort215();
void ort216();
void ort217();
void ort218();
void ort219();
void ort220();
void ort221();
void ort222();
void ort223();
void ort224();
void ort225();
void ort226();
void ort227();
void ort228();
void ort229();
void ort230();
void ort231();
void ort232();
void ort233();
void ort234();
void ort235();
void ort236();
void ort237();
void ort238();
void ort239();
void ort240();
void ort241();
void ort242();
void ort243();
void ort244();
void ort245();
void ort246();
void ort247();
void ort248();
void ort249();
void ort250();
void ort251();
void ort252();
void ort253();
void ort254();
void ort255();
void ort256();
void ort257();
void ort258();
void ort259();
void ort260();
void ort261();
void ort262();
void ort263();
void ort264();
void ort265();
void ort266();
void ort267();
void ort268();
void ort269();
void ort270();
void ort271();
void ort272();
void ort273();
void ort274();
void ort275();
void ort276();
void ort277();
void ort278();
void ort279();
void ort280();
void ort281();
void ort282();
void ort283();
void ort284();
void ort285();
void ort286();
void ort287();
void ort288();
void ort289();
void ort290();
void ort291();
void ort292();
void ort293();
void ort294();
void ort295();
void ort296();
void ort297();
void ort298();
void ort299();
void ort300();
void ort301();
void ort302();
void ort303();
void ort304();
void ort305();
void ort306();
void ort307();
void ort308();
void ort309();
void ort310();
void ort311();
void ort312();
void ort313();
void ort314();
void ort315();
void ort316();
void ort317();
void ort318();
void ort319();
void ort320();
void ort321();
void ort322();
void ort323();
void ort324();
void ort325();
void ort326();
void ort327();
void ort328();
void ort329();
void ort330();
void ort331();
void ort332();
void ort333();
void ort334();
void ort335();
void ort336();
void ort337();
void ort338();
void ort339();
void ort340();
void ort341();
void ort342();
void ort343();
void ort344();
void ort345();
void ort346();
void ort347();
void ort348();
void ort349();
void ort350();
void ort351();
void ort352();
void ort353();
void ort354();
void ort355();
void ort356();
void ort357();
void ort358();
void ort359();
void ort360();
void ort361();
void ort362();
void ort363();
void ort364();
void ort365();
void ort366();
void ort367();
void ort368();
void ort369();
void ort370();
void ort371();
void ort372();
void ort373();
void ort374();
void ort375();
void ort376();
void ort377();
void ort378();
void ort379();
void ort380();
void ort381();
void ort382();
void ort383();
void ort384();
void ort385();
void ort386();
void ort387();
void ort388();
void ort389();
void ort390();
void ort391();
void ort392();
void ort393();
void ort394();
void ort395();
void ort396();
void ort397();
void ort398();
void ort399();
void ort400();

// --------------------------------
// Zeigerliste aller ort-Funktionen
// --------------------------------
void (*raumptr[]) (void) = {vorwort, ort1, ort2, ort3, ort4, ort5, ort6, ort7, ort8, ort9, ort10, ort11, ort12, ort13, ort14, ort15, ort16, ort17, ort18, ort19, ort20, ort21, ort22, ort23, ort24, ort25, ort26, ort27, ort28, ort29, ort30, ort31, ort32, ort33, ort34, ort35, ort36, ort37, ort38, ort39, ort40, ort41, ort42, ort43, ort44, ort45, ort46, ort47, ort48, ort49, ort50, ort51, ort52, ort53, ort54, ort55, ort56, ort57, ort58, ort59, ort60, ort61, ort62, ort63, ort64, ort65, ort66, ort67, ort68, ort69, ort70, ort71, ort72, ort73, ort74, ort75, ort76, ort77, ort78, ort79, ort80, ort81, ort82, ort83, ort84, ort85, ort86, ort87, ort88, ort89, ort90, ort91, ort92, ort93, ort94, ort95, ort96, ort97, ort98, ort99, ort100, ort101, ort102, ort103, ort104, ort105, ort106, ort107, ort108, ort109, ort110, ort111, ort112, ort113, ort114, ort115, ort116, ort117, ort118, ort119, ort120, ort121, ort122, ort123, ort124, ort125, ort126, ort127, ort128, ort129, ort130, ort131, ort132, ort133, ort134, ort135, ort136, ort137, ort138, ort139, ort140, ort141, ort142, ort143, ort144, ort145, ort146, ort147, ort148, ort149, ort150, ort151, ort152, ort153, ort154, ort155, ort156, ort157, ort158, ort159, ort160, ort161, ort162, ort163, ort164, ort165, ort166, ort167, ort168, ort169, ort170, ort171, ort172, ort173, ort174, ort175, ort176, ort177, ort178, ort179, ort180, ort181, ort182, ort183, ort184, ort185, ort186, ort187, ort188, ort189, ort190, ort191, ort192, ort193, ort194, ort195, ort196, ort197, ort198, ort199, ort200, ort201, ort202, ort203, ort204, ort205, ort206, ort207, ort208, ort209, ort210, ort211, ort212, ort213, ort214, ort215, ort216, ort217, ort218, ort219, ort220, ort221, ort222, ort223, ort224, ort225, ort226, ort227, ort228, ort229, ort230, ort231, ort232, ort233, ort234, ort235, ort236, ort237, ort238, ort239, ort240, ort241, ort242, ort243, ort244, ort245, ort246, ort247, ort248, ort249, ort250, ort251, ort252, ort253, ort254, ort255, ort256, ort257, ort258, ort259, ort260, ort261, ort262, ort263, ort264, ort265, ort266, ort267, ort268, ort269, ort270, ort271, ort272, ort273, ort274, ort275, ort276, ort277, ort278, ort279, ort280, ort281, ort282, ort283, ort284, ort285, ort286, ort287, ort288, ort289, ort290, ort291, ort292, ort293, ort294, ort295, ort296, ort297, ort298, ort299, ort300, ort301, ort302, ort303, ort304, ort305, ort306, ort307, ort308, ort309, ort310, ort311, ort312, ort313, ort314, ort315, ort316, ort317, ort318, ort319, ort320, ort321, ort322, ort323, ort324, ort325, ort326, ort327, ort328, ort329, ort330, ort331, ort332, ort333, ort334, ort335, ort336, ort337, ort338, ort339, ort340, ort341, ort342, ort343, ort344, ort345, ort346, ort347, ort348, ort349, ort350, ort351, ort352, ort353, ort354, ort355, ort356, ort357, ort358, ort359, ort360, ort361, ort362, ort363, ort364, ort365, ort366, ort367, ort368, ort369, ort370, ort371, ort372, ort373, ort374, ort375, ort376, ort377, ort378, ort379, ort380, ort381, ort382, ort383, ort384, ort385, ort386, ort387, ort388, ort389, ort390, ort391, ort392, ort393, ort394, ort395, ort396, ort397, ort398, ort399, ort400 };

// -----------
// Der Spieler
// -----------
static charakter_s spieler;

// ----------------------
// Variablen des Spielers
// ----------------------

enum objektsynonyme { nichts, adamantpickel, bogensilberpfeil, buch, drachenfeuerzauber, gewandheitstrank, gewehr, glueckstrank, goldstuecke, handschuh, helm, holzhammer, holzpflock, holzschild, juwelenauge, kaese, kerze, laterne, lederruestung, lederruestung2, ohrringe, patrone, pergament, pistole, proviant, rucksack, schild, schwert, schwert2, silberkreuz, silbersichel, staerketrank, taschenlampe, taschenmesser, unsichtbarkeitstrank, verbandskasten, verwuenschterhelm, warndreieck, wunderwasser, zombiegold ,maximalobjekt };

static int objekt[maximalobjekt] = { [laterne] = 1, [lederruestung] = 1, [proviant] = 10, [rucksack] = 1, [schwert] = 1 };
static char *objektname[maximalobjekt] = { "Nichts", "Adamantpickel", "Bogen und Silberpfeil", "Buch", "Drachenfeuerzauber", "Gewandheitstrank", "Gewehr", "Glückstrank", "Goldstücke", "Handschuh", "Helm", "Holzhammer", "Holzpflock", "Holzschild", "Juwelenauge", "Käse", "blaue Kerze", "Laterne", "Lederrüstung", "Lederrüstung", "Ohrringe", "Patronen", "Pergament", "Pistole", "Proviant", "Rucksack", "Schild", "Schwert", "Zauberschwert +2", "Silberkreuz", "Silbersichel", "Stärketrank", "Pumptaschenlampe", "Taschenmesser", "Unsichtbarkeitstrank", "Verbandskasten", "Verwuenschter Helm", "Warndreieck",  "Wunderwasser", "Zombiegold" };

static int angriffsbonus = 0;
static int fuenfwahl = 0;
static bool nursilbertrifft = false;
static int paralysiert = 0;
static int preis = 0;
static int schluessel = 0;
static bool schluessel9 = false;
static bool schluessel66 = false;
static bool schluessel99 = false;
static bool schluessel111_1 = false;
static bool schluessel111_2 = false;
static bool schluessel125 = false;
static bool schluesselbootshaus = false;
static bool schummeln = false;
static bool silberwaffe = false;
static bool unsichtbar = false;
static bool tripodgesehen = false;
static int getoetetemenschen = 0;
static int getoetetegegner = 0;
static bool agartha = false;
static bool verzeichnisgelesen = false;
static bool buchgefunden = false;
static bool kartegefunden = false;
static bool sargverschoben = false;
static bool durchganggeoeffnet = false;
static bool schluesselgefunden = false;
static bool dreistelzer = false;
static bool dracheverletzt = false;
static bool drachetot = false;
static int minenzwerge = 158;
static int stollentroll = 150;
static bool gitteroffen = false;
static bool raetsel1 = false;
static bool raetsel2 = false;
static bool raetsel3 = false;
static bool raetsel4 = false;
static bool raetsel5 = false;
static bool dwellmer = false;
static int arianna = 0;
static int elke = 0;
static bool schluesselarianna = false;
static int verloben = 0;

static unsigned int rotation = 0; // Rotation ist eine Besonderheit. Hierüber werden die beiden Drehraumsegmente gesteuert. ^.^

// -------------
// Raum Variable
// -------------

static int raum = 0;

// -----------------------------------------
// Spezialmonster (immun außer gegen Silber)
// -----------------------------------------

charakter_s mechanicher_geher = { "mechanicher Geher", 14, 13, 12, 12 };

// --------------
// externe Module
// --------------

extern void beenden(enum farben f, int status, char* text, ...); // Funktion: Farbige Beenden-Funktion

extern void hintergrundfarbe(enum farben); // Funktion: Hintergrundfarbe ändern

extern void hinweis(enum farben f, char* text, ...); // Funktion: Hinweis - für Fehlermeldungen oder ähnliches

extern bool janeinfrage(char *); // Funktion: Ja-Nein-Frage

extern void ncurses_init(void (*)()); // Initialisierung der ncurses-Umgebung

extern char taste(void); // Funktion: Taste

extern void textausgabe(char *, ...); // Funktion: Textausgabe

extern void texteingabe(char *, unsigned int); // Funktion: Texteingabe

extern void vordergrundfarbe(enum farben); // Funktion: Vordergrundfarbe ändern

extern int waehle(char*, int); // Funktion: waehle

extern void weiter(void); // Funktion: Weiter

extern int wuerfel(unsigned int); // Funktion: Wuerfel

extern void zufall_per_zeit(void); // Initialisierung der Zufallszahlen

#endif // _STEAMPUNKFFR_H_

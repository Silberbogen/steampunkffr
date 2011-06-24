/*
 * =====================================================================================
 *
 *       Filename:  steampunkffr.c
 *
 *    Description:  Steampunk FFR - Der Anfang
 *    				Ein "Das-ist-dein-Abenteuer"-Roman
 *
 *        Version:  0.011
 *    letzte Beta:  0.000
 *        Created:  22.05.2011 09:35:00
 *          Ended:  00.00.0000 00:00:00
 *       Revision:  none
 *       Compiler:  clang
 *        compile:  clang -o steampunkffr steampunkffr.c -lncurses
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
 *   - 22.05.2011 Beginn an der Arbeit des Spiels
 *   - 22.05.2011 Schusswaffen und Munition implementiert
 *   - 22.05.2011 Tripodenauftauchen eingebaut
 *   - 24.06.2011 Eine Auswahl fehlte bei ort1()
 *   - 24.06.2011 Beginne damit, eine erweiterten Stadtplan einzusetzen
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

#define DATEINAME ".steampunkffrsicherung.txt"

// Neuer Typ: charakter_t & npc_t
// Zur Speicherung der Kampfwerte

typedef struct _charakter {
	char name[30];
	int gewandheit;
	int gewandheit_start;
	int staerke;
	int staerke_start;
	int glueck;
	int glueck_start;
} charakter_t;

// -----------
// Der Spieler
// -----------
static charakter_t spieler;

// ----------------------
// Variablen des Spielers
// ----------------------

enum objektsynonyme { nichts, astgabel, bogensilberpfeil, buch, drachenfeuerzauber, gewandheitstrank, gewehr, glueckstrank, goldstuecke, handschuh, helm, holzhammer, holzpflock, holzschild, juwelenauge, kaese, kerze, laterne, lederruestung, lederruestung2, ohrringe, patrone, pergament, pistole, proviant, rucksack, schild, schwert, schwert2, silberkreuz, silbersichel, staerketrank, taschenlampe, taschenmesser, unsichtbarkeitstrank, verbandskasten, verwuenschterhelm, warndreieck, wunderwasser, zombiegold ,maximalobjekt };

static int objekt[maximalobjekt] = { [laterne] = 1, [lederruestung] = 1, [proviant] = 10, [rucksack] = 1, [schwert] = 1 };
static char *objektname[maximalobjekt] = { "Nichts", "Astgabel", "Bogen und Silberpfeil", "Buch", "Drachenfeuerzauber", "Gewandheitstrank", "Gewehr", "Glückstrank", "Goldstücke", "Handschuh", "Helm", "Holzhammer", "Holzpflock", "Holzschild", "Juwelenauge", "Käse", "blaue Kerze", "Laterne", "Lederrüstung", "Lederrüstung", "Ohrringe", "Patronen", "Pergament", "Pistole", "Proviant", "Rucksack", "Schild", "Schwert", "Zauberschwert +2", "Silberkreuz", "Silbersichel", "Stärketrank", "Pumptaschenlampe", "Taschenmesser", "Unsichtbarkeitstrank", "Verbandskasten", "Verwuenschter Helm", "Warndreieck",  "Wunderwasser", "Zombiegold" };

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

// -------------
// Raum Variable
// -------------

static int raum = 0;

// ----------------
// Beenden Variable
// ----------------

bool beenden = false;

// -----------------------------------------
// Spezialmonster (immun außer gegen Silber)
// -----------------------------------------

charakter_t mechanicher_geher = { "mechanicher Geher", 14, 13, 12, 12 };

// -------------------
// Funktionsprototypen
// -------------------

// Funktion: Wuerfel
int wuerfel(unsigned int);

// Funktion: Teste dein Glück
bool tdg(charakter_t *);

// Funktion: Kampfrunde
bool kampfrunde(charakter_t *, charakter_t *, void (*)());

// Funktion: Kampf
bool kampf(charakter_t *, charakter_t *, int, bool, void (*)());

// Funktion: Momentane Werte
void momentane_werte(charakter_t *);

// Funktion: Taste
char taste(void);

// Funktion: Textausgabe
void textausgabe(char *);

// Funktion: Weiter
void weiter(void);

// Funktion: Auswahl
void auswahl(char *, int, void (*)(), void (*)(), void (*)(), void (*)(), void (*)(), void (*)());

// Funktion: VersucheDeinGlueck
void versuchedeinglueck(void (*)(), void (*)());

// Funktion: VersucheDeineGewandheit
void versuchedeinegewandheit(void (*)(), void (*)());

// Funktion: waehle
int waehle(char*, int);

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

// Funktion: Ja-Nein-Frage
bool janeinfrage(char *);

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
void quit(void);

// ---------------------------------
// Implementation für das Spielmodul
// ---------------------------------

void vorwort();
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
void (*raumptr[401]) (void) = {vorwort, ort1, ort2, ort3, ort4, ort5, ort6, ort7, ort8, ort9, ort10, ort11, ort12, ort13, ort14, ort15, ort16, ort17, ort18, ort19, ort20, ort21, ort22, ort23, ort24, ort25, ort26, ort27, ort28, ort29, ort30, ort31, ort32, ort33, ort34, ort35, ort36, ort37, ort38, ort39, ort40, ort41, ort42, ort43, ort44, ort45, ort46, ort47, ort48, ort49, ort50, ort51, ort52, ort53, ort54, ort55, ort56, ort57, ort58, ort59, ort60, ort61, ort62, ort63, ort64, ort65, ort66, ort67, ort68, ort69, ort70, ort71, ort72, ort73, ort74, ort75, ort76, ort77, ort78, ort79, ort80, ort81, ort82, ort83, ort84, ort85, ort86, ort87, ort88, ort89, ort90, ort91, ort92, ort93, ort94, ort95, ort96, ort97, ort98, ort99, ort100, ort101, ort102, ort103, ort104, ort105, ort106, ort107, ort108, ort109, ort110, ort111, ort112, ort113, ort114, ort115, ort116, ort117, ort118, ort119, ort120, ort121, ort122, ort123, ort124, ort125, ort126, ort127, ort128, ort129, ort130, ort131, ort132, ort133, ort134, ort135, ort136, ort137, ort138, ort139, ort140, ort141, ort142, ort143, ort144, ort145, ort146, ort147, ort148, ort149, ort150, ort151, ort152, ort153, ort154, ort155, ort156, ort157, ort158, ort159, ort160, ort161, ort162, ort163, ort164, ort165, ort166, ort167, ort168, ort169, ort170, ort171, ort172, ort173, ort174, ort175, ort176, ort177, ort178, ort179, ort180, ort181, ort182, ort183, ort184, ort185, ort186, ort187, ort188, ort189, ort190, ort191, ort192, ort193, ort194, ort195, ort196, ort197, ort198, ort199, ort200, ort201, ort202, ort203, ort204, ort205, ort206, ort207, ort208, ort209, ort210, ort211, ort212, ort213, ort214, ort215, ort216, ort217, ort218, ort219, ort220, ort221, ort222, ort223, ort224, ort225, ort226, ort227, ort228, ort229, ort230, ort231, ort232, ort233, ort234, ort235, ort236, ort237, ort238, ort239, ort240, ort241, ort242, ort243, ort244, ort245, ort246, ort247, ort248, ort249, ort250, ort251, ort252, ort253, ort254, ort255, ort256, ort257, ort258, ort259, ort260, ort261, ort262, ort263, ort264, ort265, ort266, ort267, ort268, ort269, ort270, ort271, ort272, ort273, ort274, ort275, ort276, ort277, ort278, ort279, ort280, ort281, ort282, ort283, ort284, ort285, ort286, ort287, ort288, ort289, ort290, ort291, ort292, ort293, ort294, ort295, ort296, ort297, ort298, ort299, ort300, ort301, ort302, ort303, ort304, ort305, ort306, ort307, ort308, ort309, ort310, ort311, ort312, ort313, ort314, ort315, ort316, ort317, ort318, ort319, ort320, ort321, ort322, ort323, ort324, ort325, ort326, ort327, ort328, ort329, ort330, ort331, ort332, ort333, ort334, ort335, ort336, ort337, ort338, ort339, ort340, ort341, ort342, ort343, ort344, ort345, ort346, ort347, ort348, ort349, ort350, ort351, ort352, ort353, ort354, ort355, ort356, ort357, ort358, ort359, ort360, ort361, ort362, ort363, ort364, ort365, ort366, ort367, ort368, ort369, ort370, ort371, ort372, ort373, ort374, ort375, ort376, ort377, ort378, ort379, ort380, ort381, ort382, ort383, ort384, ort385, ort386, ort387, ort388, ort389, ort390, ort391, ort392, ort393, ort394, ort395, ort396, ort397, ort398, ort399, ort400 };

// -------------------------
// Das eigentliche Abenteuer
// -------------------------
void intro(void) {
	int eingabe = 0;

	color_set(0, 0);
	textausgabe("Hinweis\nIn diesem Roman wird niemand anderes als du selbst die Person sein, die das Abenteuer durchlebt. Von daher würde ich sagen, würdest du dir selber beim Erleben der Spielatmosspähre helfen, wenn du die Spielfigur nach dir benennst, oder ihr einen Namen gibst, der dir gefällt oder den du gerne tragen würdest.\nViel Spaß beim Lesen und Spielen!\nSascha");
	color_set(1, 0);
	printw("Welchen Namen möchtest du deinem Abenteurer geben? ");
	getnstr(spieler.name, 30);
	color_set(0, 0);
	spieler.gewandheit_start = wuerfel(6) + 6;
	spieler.gewandheit = spieler.gewandheit_start;
	spieler.staerke_start = wuerfel(6) + wuerfel(6) + 12;
	spieler.staerke = spieler.staerke_start;
	spieler.glueck_start = wuerfel(6) + 6;
	spieler.glueck = spieler.glueck_start;
	textausgabe("Zu Beginn dieses Abenteuer wirst du nur ein absolutes Minimum an Objekten bei dir führen, als da wären ein Rucksack, ein Multifunktionstaschenmesser, eine Pumptaschenlampe und etwas Proviant. Außerdem ein Engergydrink, den du dir selber auswählen kannst. Welchen der Energydrinks wählst du?\n(1) den Gewandheitstrank - er stellt die anfängliche Gewandheitspunktzahl wieder her\n(2) den Stärketrank - er stellt die anfängliche Stärkepunktzahl wieder her\n(3) den Glückstrank - er stellt die anfängliche Glückspunktzahl wieder her und verbesser sie zusätzlich um einen Punkt");
	while((eingabe < 1) || (eingabe > 3)) {
		eingabe = waehle("Für welchen Energydrink entscheidest du dich? ", 3);
		switch(eingabe) {
			case 1: objekt[gewandheitstrank] += 2;
					break;
			case 2: objekt[staerketrank] += 2;
					break;
			case 3: objekt[glueckstrank] += 2;
					break;
			default: color_set(4, 0);
					 printw("Unerklärbarer Fehler! In der Energydrink-Auswahl,  in Funktion intro().");
					 color_set(0, 0);
					 break;
		}
	}
}

void vorwort(void) {
	textausgabe("Vorwort\n\nViele Menschen würden der Wiedervereinigung und der Politik die Schuld geben, das Menschen ins Abseits geraten. Die meisten wissen nicht, was das wirklich bedeutet, du jedoch schon. Seit Jahren bereits kämpfst du um dein Überleben, mit nicht mehr als deiner Kleidung am Körper, einem Ruckscack, einem Zelt, einer Pumptaschenlampe, einem Multifunktionstaschenmesser billigster Machart und einigen wenigen weiteren Kleinigkeiten. Momentan ist das Wetter noch Mild - und so hast du die Nacht in den Überresten einer alten Turmruine verbracht, die unbeachtet im Park des Museums am Münster steht.\nDoch diese Nacht war anders, als jede andere Nacht, die du im Freien verbracht hast. Sie war unheimlich. Du kannst dich erinnern, wie du aus dem Zelt klettertest - aber es war noch dunkel, und pechschwarz. Der ganze Himmel, alles war nur Schwarz, kein einziger Stern war zu sehen, nicht die Silhouette des Mondes, keine Wolken - rein gar nichts! Und dann war da dieses röhrende, metallische Geräusch. Und noch bevor du weißt, wie dir geschieht, wird es langsam hell - und du stehst immer noch da, in der gleichen Position. Bewegungslos. Die Frage ist nur, wie lange schon hast du bewegungslos herumgestanden? Und wieso überhaupt konntest du dich nicht bewegen?\nDa es noch immer recht früh ist, gehst du hinunter zu dem kunstvollen Brunnen des Museums. Der Park gehört in diesen Stunden dir allein. Du nimmst ein kurzes Bad und fühlst dich direkt viel wohler, so sauber, wenn es auch ein klein wenig eiskalt war. Mit einem alten Handtuch reibst du dich notdürftig trocken und ziehst dir saubere Unterwäsche an, bevor der Rest deiner alltäglichen Kleidung folgt. Den Rest deiner Kleidung verstaust du in einem Stoffbeutel und gehst zurück zu deinem Zelt. Mit wenigen Handgriffen baust du es auseinander, dann faltest du es und verstaust es vorsichtig.\nMit deinem ganzen Habseligkeiten bepackt gehst du die Treppen des Münster hinunter. Als Kind hast du dafür immer den schnellen Weg gewählt, mit den glatten Sandahlensohlen seid deine Freunde und ihre einfach immer stehend das Geländer hinuntergerutscht. Kinder kenen keinerlei Angst vor Gefahren. Du überquerst den Platz hinüber in Richtung des ehemaligen Dortin-Hotels und schlägst den Weg, die Hügelstraße hinauf ein. Nahe dem alten Wasserturm am Odenkirchweg versteckst du den Rucksack mit deinem Zelt, der Isoliermatte und all den anderen Dingen, die du nur des Nachts brauchst - und wanderst nur mit deinem \"leichten Gepäck\" die Vitusstraße hinunter. Du wanderst die Lüperzender Straße entlang und gehst schließlich die Brücke am ehemaligen Zentralbad hinauf - um noch kurz den kleine Park zu durchqueren, bevor du in Kürze die Hindenburgstraße nahe des Stadttheaters, oder einem seiner Nachfahren erreichst.");
}

// -----------
// --- 001 ---
// -----------

void ort1(void) {
	raum = 1;
	textausgabe("Die Innenstadt macht um diese Zeit noch einen gänzlich unbelebten Eindruck - und kein einiges Licht erhellt die Straße. Keine Straßenlaterne, keine Glühbirne hinter einem Fenster, nicht einmal die Werbetafeln leuchten auf. Wenn du so drüber nachdenkst, auf deinem ganzen Weg hierhin hast du bisher keinerlei eingeschaltete Lichtquellen gesehen.\nNun stehst du an der Kreuzung Stepgesstraße Ecke Hindenburgstraße. Ein Stück die große Einkaufsstraße hinauf ist das Bäckereigeschäft, an dem dir jeden Morgen ein guter Geist eine braune Papiertüte mit dampfend warmen Gebäck bereitstellt. Du schaust in die Richtung, doch diesmal steht auf dem Stromkasten neben der Türe nicht der verlockende Beutel, sondern eine Gestalt steht in der Türe und winkt dir zu.");
	auswahl("Wirst du auf die winkende Gestalt zugehen (1) oder ziehst du es vor, dich lieber scheu und mit knurrendem Magen aus dem Staub zu machen (2)?", 2, ort2, ort3, NULL, NULL, NULL, NULL);
}

void ort2(void) {
	raum = 2;
	textausgabe("Vorsichtig und etwas nervös näherst du dich der offenstehenden Ladentüre, als ein Regelrechter Platzregen losbricht, dann hört man es auch donnern. Jetzt läufst du auf die Türe zu. Eine Frau im weißen Kitel geht hinein und hält sie dir offen. Du gleitest hinein und bleibst neben einem Stehtisch stehen, während sie hinter dir die Türe zumacht und abschließt.\n\"Ist das nicht ein unheimlicher Morgen? Die ganze Stadt ist ohne Strom. Momentan habe ich ein Feuer im alten Backofen gemacht - gleich wird es hier ein paar frische Brötchen geben.\" sagt sie und geht an der Theke vorbei auf eine Tür die nach hinten führt zu. \"Na kommen sie!\" lädt sie dich ein, ihr zu folgen. Du gehst unsicher hinterher. Im Hinterzimmer steht ein Tisch, auf dem eine Thermoskanne und zwei Tassen bereitstehen. \"Ich wollte sie schon längst einmal angesprochen haben. Daß das an einem so schlechten Tag passiert, tut mir leid!\" Sie kommt mit einem Tablet voller dampfender Brötchen aus einem Nebenraum, während du unschlüssig neben dem Tisch stehst.\n\"Bitte setzen sie sich doch! Ich dachte, wir könnten zusammen frühstücken!\" sagt sie und stellt das Tablett auf dem Tisch ab. Dann setzt sie sich hin. \"Ich bin Elke.\" stellt sie sich vor.\nAls du ihr gerade mit deinem Namen antworten willst, geschieht das unfassbare: die Sirenen fangen an zu heulen, mitten in dieser Kackophonie des Gewitters - und das um Fünf Uhr, Morgens in der Frühe.\n\"Was geht da vor sich?\" fragt Elke, während du dir das Sirenensignal genauer anhörst. \"Haben sie ein Radio hier, oder einen Fernsehe?\" fragst du Elke, aber die zuckt nur mit den Schultern.\n\"Kein Batteriebetriebenes, der Strom ist ausgefallen, da tut's der Fernseher nicht.\" Sie deutete mit der Hand neben deinem Ohr entlang hinter dich. Du drehst dich auf der Sitzfläche des Stuhls um und erblickst das Gerät. Die Sirenen heulen jetzt bereits seit mehr als einer Minute.\n\"Das ist keine Übung, das ist eine echte Warnung, aber vor was?\" sagst du laut zu dir. Du sprichst sehr oft mit dir selbst, vermutlich, um nicht vor Einsamkeit den Verstand zu verlieren. Wieder erklingt das Donnern und Grollen.");
	auswahl("Willst du erst einmal in Ruhe Frühstücken (1) oder Elke fragen, ob sie dich rauslassen kann, damit du nachsiehst, was eigentlich los ist (2) oder sie fragen, ob sie über einen weiteren Empfänger verfügt, der nicht ans Stromnetz angeschlossen ist (3)?", 3, ort4, ort5, ort6, NULL, NULL, NULL);
}

void ort3(void) {
// Kein Kontakt
	raum = 3;
	textausgabe("Du bist ein äußerst mißtrauischer Mensch und vertraust der unbekannten, winkenden Gestalt nicht. Zu deinem Glück fängt es an zu regnen. Es ist ein echter Platzregen. Du rennst ein Stück weit die Hindenburgstraße hinab und biegst in die Lichtpassage ein, wo du dich während des Regens unterstellts. Kaum bist du eine Minute drin, als plötzlich überall die Sirenen anfangen zu heulen. Draußen hörst du ein lautes Donnergrollen. Auch ist es viel dunkler geworden, seit die dichten grauen Wolken sich mit ihrem Regen auf Gladbach ergehen. Plötzlich pfeift ein Schuß an dir vorbei und schlägt in die Fensterscheibe ein Stück vor dir ein. Du hechtest in den nächsten Eingang und siehst zurück. Du bist nicht alleine. Von oben herab kommt eine Gestalt gelaufen. Aus deinem Rucksack holst du dein Taschenmesser heraus und klappst es auf. Es ist fast so, als wärst du unbewaffnet, aber besser als nichts. Dein Gegner kommt weiter auf dich zugeeilt.");
	ort13(); // Zufallsgegner
	textausgabe("Die Pistole deines Gegners ist leergeschossen. Du durchsuchst ihn hastig, findest, aber keinerlei Munition bei ihm.");
	if(janeinfrage("Möchtest du die Pistole einstecken (j/n)?"))
		objekt[pistole] += 1;
	textausgabe("Es war zwar Notwehr, dennoch hockst du jetzt hier neben einer Leiche. Dein Gefühl sagt dir, daß es wohl das beste wäre, dich so schnell wie möglich aus dem Staub zu machen, Unwetter hin oder her.");
	auswahl("Möchtest du den Lichthof nach Norden verlassen in Richtung Kaisterstraße (1), oder nach Süden in Richtung Hindernburgstraße (2)?", 2, ort50, ort51, NULL, NULL, NULL, NULL);
}

void ort4(void) {
// Frühstücken
	raum = 4;
	textausgabe("Du setzt dich zu Elke an den Tisch. Das Wetter, den Donner und den fehlenden Strom ignorieren, lernt ihr euch langsam besser kennen, während die Anzahl der Backwaren auf dem Tablett deutlich schrumpfen und der heiße Kaffee aus der Thermoskanne sich wohlig in deinem Inneren verteilt. Der Ort, die Zeit, die Situation, alles könnte man als idyllisch bezeichnen, wenn, ja wenn nicht auf einmal dieses seltsame Geräusch eine absterbenden Sirene gewsen wäre. Es war kein Abschwellen, wie man es kenn, sondern klang eher, wie ein entenartiger Aufschrei.\nUnd dann nimmst du plötzlich war, das mit dem Verstummen dieser Sirene die Masse an Sirenen, die noch klingen, weniger hoch ist, als zuvor. Aber was viel wichtiger ist, einige gehen immer noch. Langsam wirst du dir der Situation bewußt, die da draußen herrscht - und du beschließt, nachsehen zu gehen, was da los ist.");
	ort5();
}

void ort5(void) {
// Raus auf die Straße
	raum = 5;
	textausgabe("Du schlägst vor, daß du dich draußen umsiehst und zurückkommst, sobald du weißt, was los ist. Elke begleitet dich zur Ladentüre und läßt dich raus. Der Regen prasselt von oben herab und immer wieder donnert es. Du winkst ihr kurz zu und rennst so gut es geht an den Hauswänden entlang die Hindenburgstraße hinauf, trotzdem bist du nach weniger als einer Minute bis auf die Unterhose durchnäßt. Als du am ehemaligen Heinemann vorbeikommst und durch die kurze Passage läuftst, bemerkst du an der Straßenecke zum Sankt Vith hinunter einen brennenden Polizeiwagen. Ein mulmiges Gefühl geht dir durch den Magen. Eigentlich wolltest du ja in das Haus, das früher deinem Großvater gehört hat - und von dem aus man eine Übersicht über die ganze Stadt hat. Trotzdem ergreift dich gerade die Angst.");
	auswahl("Vielleicht wäre es ja besser, die Straße wieder hinunter zulaufen - und sich im Geschäft bei Elke zu verstecken (1)? Du könntest auch auf den Polizeiwagen zulaufen, vielleicht kannst du jemandem helfen, der im Wagen eingeklemmt ist (2)? Oder du folgst deinem ursprünglichen Plan, das Haus deines Großvaters zu erreichen (3)?", 3, ort7, ort8, ort9, NULL, NULL, NULL);
}

void ort6(void) {
// Nach einem Empfänger fragen
	raum = 6;
	textausgabe("Elke schüttelt den Kopf, und verneint deine Frage. \"Tut mir leid, so etwas ist nicht hier im Geschäft. Das einzige Radio, das mir einfallen würde, wäre mein Autoradio.\" entgegnet sie, nimmt sich ein warmes Brötchen vom Tablett und kaut daran herum.");
	auswahl("Willst du sie wirklich überreden, mit dir zu ihrem Auto zu gehen (1), oder würdest du jetzt nicht viel lieber frühstücken (2)? Wenn du unbedingt draußen nachsehen willst, was los ist, könntest du Elke auch bitten, dich rauszulassen (3).", 3, ort17, ort4, ort5, NULL, NULL, NULL);
}

void ort7(void) {
// Zurück zum Geschäft
	raum = 7;
	textausgabe("Vollkommen durchnäßt kommst du wieder am Geschäft an. Drinnen ist alles dunkel. Du klopfst mehrfach, aber nichts rührt sich.");
	auswahl("Willst du es weiter mit klopfen und rufen probieren (1), oder willst du versuchen, ob du die Türe öffnen kannst (2), oder willst du dir einen anderen Weg suchen (3)?", 3, ort14, ort15, ort16, NULL, NULL, NULL);
}

void ort8(void) {
// Zum Polizeiwagen
	raum = 8;
	if(objekt[gewehr] > 0) {
		textausgabe("Du läufst erneut über den Platz um dir den Polizeiwagen noch einmal anzusehen. Das Feuer ist erloschen. Du überlegst dir gerade, ob du eine noch gründlichere Durchsuchung des Wagens vornehmen sollst, als du ein Geräusch aus der Richtung der Kirche hören kannst. Ein Geschoss schlägt in die Karosserie des Wagens ein.");
		auswahl("Willst du dich dem Kampf stellen (1) oder versuchen zu fliehen (2)?", 2, ort10, ort11, NULL, NULL, NULL, NULL);
	}
	else {
		textausgabe("Der Polizeiwagen raucht eigentlich nur noch. Fahrer- und Beifahrertüre stehen weit offen. Vorsichtig umrundest du den Wagen. Kein Mensch ist darin, aber auch kein Anzeichen der beiden Polizisten, die sich darin hätte befinden müssen. Im offenen Seitenfach der Beifahrertüre ist ein Gewehr, die Verriegelung der Waffe ist offen. Du könntest sie an dich nehmen. Du durchsuchst den Kofferraum. In diesem befindet sich ein Munitionspäckchen mit 100 Patronen, ein Verbandskasten und ein Warndreieck.");
		if(janeinfrage("Willst du etwas von den Sachen mitnehmen (j/n)?")) {
			if(janeinfrage("Willst du das Gewehr mitnehmen (j/n)?"))
				objekt[gewehr] += 1;
			if(janeinfrage("Willst du die Patronen mitnehmen (j/n)?"))
				objekt[patrone] += 100;
			if(janeinfrage("Willst du den Verbandskasten mitnehmen (j/n)?"))
				objekt[verbandskasten] += 1;
			if(janeinfrage("Willst du das Warndreieck mitnehmen (j/n)?"))
				objekt[warndreieck] += 1;
		}
		auswahl("Willst du jetzt weiter zum Haus deines Großvaters (1) oder zurück zum Geschäft (2)?", 2, ort9, ort7, NULL, NULL, NULL, NULL);
	}
}

void ort9(void) {
// Zum Haus deines Großvaters
	raum = 9;
	if(tripodgesehen == true) {
		textausgabe("Du läufst auf das Haus deines Großvaters zu und in den Eingang zwischen Gaststätte und Kleidungsgeschäft. Für einen Moment mußt du vor der alten Holztüre mit der Glasscheibe halten. Mit Hilfe deines Taschenmessers daudert es nur ein paar Sekunden, dann ist die Türe öffnet und du schlüpfst hinein.");
		ort21();
	}
	textausgabe("Du läufst an der Häuserwand des Alten Marktes entlang und gelangst schließlich zum Haus deines Großvaters. Du machst dich mit Hilfe deines Taschenmessers kurz am Schloß der alten Holztüre zu schaffen, sie geht fast sofort aus. Den Trick hast du schon als Kind gemacht, wenn du mal wieder deinen Schlüssel zu Hause vergessen hattest - und er klappt immer noch wunderbar. Du hastest die Türe hinauf. Immer noch donnert es draußen, so laut, wie du es schon lange bei keinem Gewitter mehr gehört hast. Auf jeder Etage drückst du den Lichtschalter, aber keiner schaltet das Licht an. Auch hier ist vollkommener Stromausfall. Kurz bevor du die oberste Etage erreichst, hören die Sirenen auf zu heulen, was für dich nur zum Vorteil sein kann, steht doch noch eine alte motorbetriebene E57. Du obersten Treppen sind am kürzesten und am verwinkelsten. Links die Waschküche läßt du liegen, da kannst du nichs sehen. Du stürmst nach rechts, den kurzen Gang entgang und reißt die hintereste Türe auf der rechten Seite auf. Du stürmst auf das Dachfenster an der Südseite zu. Überall siehst du dunkelgraue Wolkenberge sich auftürmen. Statt bis nach Düsseldorf kannst du nicht einmal bis an die Westgrenze Mönchengladbachs sehen. Du wendest den Blick ab und läufst zu einem der Fenster an der Nordseite. Hier bietet sich die ein gleiches Bild. Die Wolken sind so dicht, daß du nicht einmal den Gladbacher Wasserturm in dieser Brühe sehen kannst. Bleiben noch die Fenster an der Südseite. Bereits als du dich ihnen näherst, erkennst du, daß du hier ein weiteres Sichtfeld haben wirst. Du reißt das Fenster auf um besser sehen zu können. Von oben peitschen dicke Regentropen herab, aber das aufgeklappte Fenster schützt dich weitestgehend. Die Wolkenwand ist hier einige Kilometer entfernt. Da plötzlich wird es hell in der Wolkenwand. Wie gebannt starrst du dahin. War das ein Blitz? Da wieder. Wieder ein Blitz. Wieder in der Wolkenwand. Das ist jetzt aber sehr ungewöhnlich. Minutenlang starrst du auf die Wolkenwand - und auch an zwei oder drei anderen Stellen erblickst du immer wieder kurze Blitze - mitten in der Wolkenwand, aber fast auf Bodenhöhe. Ein mulmiges Gefühl breitet sich in deinem Magen aus. Gerade, als du das Fenster schließen willst, kommt etwas aus der Wolke. Etwas riesiges. Ein Objekt, wie ein Turm, aber es bewegt sich, wie ein Stelzenläufer. Und ein Lichtbogen, wie ein Blitz spannt sich von ihm aus, dann brennt etwas zu seinen - Beinen? - während es sich weiter in Richtung der Gladbacher Innenstadt voranschiebt.\n Deine Nackenhärchen haben sich aufgerichtet. Du weißt zwar nicht genau, was das ist, aber es bringt Zerstörung, soviel ist sicher. Hastig schließt du das Fenster. Du rennst aus dem Dachstuhl heraus zurück in den Flur und eilst die Treppen hinab, bis du unten an den der Haustüre ankommst. Was wirst du tun?");
	tripodgesehen = true;
	auswahl("Du läufst hinaus und zu dem Polizeiwagen (1), du läufst die Kellertreppe hinab und suchst dort Schutz vor dem was kommt (2) oder du läufst zurück zu Elke, der Frau, die dich täglich mit Backwaren versorgt hat und erzählst ihr, was du gesehen hast (3)?", 3, ort8, ort20, ort7, NULL, NULL, NULL);
}

void ort10(void) {
// Kampf
	raum = 10;
	ort13();
	textausgabe("Du bist nicht stolz darauf, einen anderen Menschen getötet zu haben, aber du warst ja nicht der Angreifer. Trotzdem fühlst du dich schäbig. Etwas in dir hat sich verändert, das kannst du spüren. Noch immer prasselt der Regen auf dich, so als wäre nichts gewesen. Und hinter den Wolkenbergen, da bist du dir sicher, scheint immer noch die Sonne.");
	auswahl("Willst du jetzt weiter zum Haus deines Großvaters (1) oder zurück zum Geschäft (2)?", 2, ort9, ort7, NULL, NULL, NULL, NULL);
}

void ort11(void) {
// fliehen
	raum = 11;
	versuchedeinglueck(ort12, ort13);
	ort10();
}

void ort12(void) {
// entkommen
	raum = 12;
	textausgabe("Geduckt rennst du an der Häuserwand entlang und dann rechts in die Straße, auf das Kabuff zu, dort rennst du links um die Ecke in Richtung Kapuzinerplatz, läufst dann aber stattdessen um die Kirche herum, ein kurzes Stück über den Parkplatz - und dann die Stepgesstraße hinunter, bis du schließlich wieder die Hindenburgstraße erreichst. Etwas atemlos stellst du dich an die Wand. Mehr als nur einmal bist du beim Laufen geschlittert. Das Wasser läuft in Strömen in die Kanalisation. Du vergewisserst dich, bleibst mehrere Minuten in deiner versteckten Position, bis du dir sicher bist, daß der Angreifer dir nicht gefolgt ist. Erst jetzt begibst du dich zurück zur Bäckerei.");
	ort7();
}

void ort13(void) {
	// Begegnung mit einem Zufallsgegner der "Zivilseite"
	int zufallsgegner = wuerfel(6);
	bool kampfausgang;
	charakter_t gegner1 = { "verängstigte Frau", 2, 2, 3, 3 };
	charakter_t gegner2 = { "Plünderer", 6, 6, 3, 3 };
	charakter_t gegner3 = { "aggressiver Jugendlicher", 6, 6, 4, 4 };
	charakter_t gegner4 = { "Polizist", 5, 5, 4, 4 };
	charakter_t gegner5 = { "Gutbürger", 6, 6, 5, 5 };
	charakter_t gegner6 = { "Heckenschütze", 8, 8, 4, 4 };
	charakter_t gegner7 = { "wahnsinniger Polizist", 6, 6, 4, 4 };
	switch(zufallsgegner) {
		case 1: kampfausgang = kampf(&spieler, &gegner1, 1, false, NULL);
				break;
		case 2: kampfausgang = kampf(&spieler, &gegner2, 1, false, NULL);
				break;
		case 3: kampfausgang = kampf(&spieler, &gegner3, 1, false, NULL);
				break;
		case 4: kampfausgang = kampf(&spieler, &gegner4, 1, false, NULL);
				break;
		case 5: kampfausgang = kampf(&spieler, &gegner5, 1, false, NULL);
				break;
		case 6: kampfausgang = kampf(&spieler, &gegner6, 1, false, NULL);
				break;
		default: kampfausgang = kampf(&spieler, &gegner7, 1, false, NULL);
				break;
	}
	if(!kampfausgang) {
		textausgabe("Das war nicht dein bester Kampf. Um ehrlich zu sein, das war dein schlechtester Kampf - und auch dein letzter Kampf. Dein allerletzter Kampf, den du nicht überlebt hast. Mit dir ist es zu ENDE gegangen.");
		exit(EXIT_SUCCESS);
	}
	getoetetemenschen += 1;
	if(wuerfel(6) >= 4) {
		if(objekt[pistole] <= 1)
			if(janeinfrage("Willst du die Pistole deines Gegners an dich nehmen (j/n)?"))
				objekt[pistole] += 1;
	}
	else {
		if(objekt[gewehr] <= 0)
			if(janeinfrage("Willst du das Gewehr deines Gegners an dich nehmen (j/n)?"))
				objekt[gewehr] += 1;
	}
	objekt[patrone] += wuerfel(8);
}

void ort14(void) {
// Klopfen und rufen
	raum = 14;
	if(wuerfel(6) >= 4) {
		textausgabe("Dein Klopfen hat dich unvorsichtig gemacht. Du hast einen unerwarteten Gast angelockt.");
		ort13();
	}
	else textausgabe("Bisher scheinen deine Bemühungen nicht zu fruchten. Drinnen reagiert Nichts und niemand auf dein Klopfen.");
	auswahl("Willst du es weiter mit klopfen und rufen probieren (1), oder willst du versuchen, ob du die Türe öffnen kannst (2), oder willst du dir einen anderen Weg suchen (3)?", 3, ort14, ort15, ort16, NULL, NULL, NULL);
}

void ort15(void) {
// Versuchen, die Türe aufzubekommen
	raum = 15;
	versuchedeinglueck(ort18, ort19);
}

void ort16(void) {
// Versuchen, einen anderen Weg zu finden
	raum = 16;
}

void ort17(void) {
// Auf dem Weg zu Elkes Auto
	raum = 17;
	textausgabe("Du schulterst dir deinen Rucksack, während Elke ein paar Papiertüten mit Backwaren füllt. Danach siehst du ihr zu, wie sie ein paar Flaschen aus dem Kühlschrank holt und in eine weitere Tüte stopft. Als sie alles gepackt hat, verschwindet sie kurz im Nebenzimmer und kommt, jetzt einen Mantel tragend, eine Damenhandtusche um die Schulter hängend zurück. Ihr nennt die ganzen Taschen und verlasst das Geschäft. Elke schließt den Laden hinter sich ab, macht sogar die Bewegung zum Scharfschalten der Alarmanlage, dann sagt sie: \"Komm!\" und geht dir voraus, die Stepgesstraße hinunter.\nMißtrauisch wirfst du einen Blick zum Jugendheim hinüber. Oft genug, haben dir da schon junge Kerle aufgelauert. Diesmal aber bleibt alles ruhig. Elke führt dich ein kurzes Stück weiter und hält vor einer Parkhaustüre. Sie holt einen Schlüssel heraus und schließt auf. Gemeinsam geht ihr in das Innere des Parkhauses. Das Treppenhaus stinkt nicht so muffig und nach Urin, wie es oft der Fall ist, wenn Betrunkene sich eine Stelle für ihre Notdurft suchen. Ihr steigt hinab bis zum dritten Untergeschoß.\nSchließlich erreicht ihr Elkes Auto, einen schon etwas älteren Kombi. Sie entriegelt das Auto per Knopf. Ihr verladet eure Taschen und deinen Rucksack im Kofferraum und setzt euch in das Auto, wobei du auf dem Beifahrersitz Platz nimmst. Das kleine Licht der Mittelkonsole ist im Moment die einzige aktive Lichtquelle im ganzen Parkhaus.\nElke fummelt am Autoradio herum, aber das Digitalradio findet keinen Sender den es darstellen kann. Schließlich gibt sie die Suche auf. \"Hier drinnen haben wir keinen Empfang, ich müßte schon aus dem Parkhaus herausfahren, damit wir etwas hören könnten.\"");
	auswahl("Stimmst du Elke zu - und ihr fahrt mit dem Auto aus dem Parkhaus heraus in den Regen (1) oder hast du ein mulmiges Gefühl und glaubst nicht, daß das eine gute Idee ist (2)?", 2, ort100, ort101, NULL, NULL, NULL, NULL);
}

void ort18(void) {
// Du bekommst die Türe auf
	raum = 18;
}

void ort19(void) {
// Die Türe geht nicht auf
	raum = 19;
}

void ort20(void) {
// Im Keller des Großvaters
	raum = 20;
	textausgabe("Du öffnest die eiserne Treppe und gehst die Kellertreppe hinunter. Als du am Fuße der Treppe ankommst, siehst du vor dir die eiserne Doppeltüre, die in den Heizungskeller führt. Nach rechts führt ein weiterer Weg zum Lagerraum, wo deine Großmutter ihre Kartoffeln lagerte. Neben der Treppe führt ein Weg nach hinten, wo die Mieter des Hauses ihre Kellerabteile haben.");
	auswahl("Wenn du zurück nach oben gehen willst (1). Möchtest du in den Heizungskeller (2). Willst du in den Gang, der zum Kartoffellager deiner Großmutter führt (3). Würdest du gerne in den rückwärtigen Bereich gehen (4).", 4, ort21, ort22, ort23, ort24, NULL, NULL);
}

void ort21(void) {
// Treppenflur
	raum = 21;
	textausgabe("Du stehst in einem großen Treppenflur. An der Seite befindet sich der Hängeschrank mit den ganzen Ablesegeräten für Elektrizität und Wasser. Rechts an der Wand für eine Steintreppe hinauf in das erste Stockwerk. Geradeaus ist eine Holztüre, durch die man in den Kellerbereich der Gaststätte kommt, allerdings ist sie dauerhaft abgeschlossen. Rechts neben der Holztüre, unterhalb der Steintreppe, befindet sich eine Eisentüre, hinter der sich der Abstieg in den Keller befindet.");
	auswahl("Willst du die Treppe hinaufsteigen (1), in den Keller hinuntergehen (2), oder das Haus verlassen und zurück auf den Alten Markt (3)?", 3, ort25, ort20, ort26, NULL, NULL, NULL);
}

void ort22(void) {
// Heizungskeller
	raum = 22;
	if(agartha == true)
		textausgabe("Ein großteil der Kohlen liegt auf dem Boden. An der rußgeschwärtzten Wand kannst du das Wort \"Agartha\" lesen. Der Heizungskessel arbeitet derzeit nicht. Sonst befindet sich ncihts im Heizungskeller.");
	else {
		textausgabe(" Als du die Türe öffnest, ist es recht stickig im Raum. Der Heizkessel ist aus, obwohl in einem großen Verschlag an der Seite sich die Kohlen nur so stapeln. Als du genauer hinguckst, hast du das Gefühl, den geritzen Buchstaben \"A\" an der Wand zu sehen.");
		if(janeinfrage("Willst du soviele der Kohlen zur Seite räumen, um zu sehen, ob das \"A\" vielleicht Teil eines Wortes ist? Eine letzte Nachricht deines Großvaters an dich? (j/n)")) {
			textausgabe("Deine Kleidung verdreckt vollständig, während du auf den Kohleberg krabbelst - und Brikett umd Brikett wegräumst. Noch dazu machst du eine Menge Krach.");
			if(wuerfel(6) >= 4)
				ort13();
			textausgabe("Endlich hast du das Gekritzel an der Wand freigelegt. Was du liest, ist ein Wort: \"Agartha\"");
			agartha = true;
		}
	}
	auswahl("Willst du in in den Gang zum Kartoffelkeller (1), oder willst du zu den rückwärtigen Kellen (2) oder möchtest du zurück nach oben in den Treppenflur (3)?", 3, ort23, ort24, ort21, NULL, NULL, NULL);
}

void ort23(void) {
// Gang zum Kartoffelkeller
	raum = 23;
	if(agartha && !schluesselgefunden) {
		textausgabe("Du gehst durch den nach Moder riechenden Gang auf das ehemalige Kartoffellager deiner Großmutter zu. Du wendest dich nach links und stehst im Türrahmen. Da erinnerst du dich. Du siehst nach links, neben dem Rahmen war ein kurzer Nagel, an dem ein Schlüssel mit einem weißumrandeten Plastik-Schlüsselkärtchen hin. Auf dem weißen Schild war mit der krakeligen Handschrift deines Großvaters dein eigener Name niedergekritzelt.\nDer Nagel steckt zwar noch in der Hand, aber er deutet nach unten - und kein Schlüssel hängt daran. Du kniest dich nieder - und durchwühlst den mit Würmern belebten Karttoffel-Schimmel-Matsch am Boden. Der Geruch steigt dir unangenehm durch die Nase. Minutenlang durchpflügst du den Matsch. Der Verzweiflung nahe willst du schon aufgeben, als dein Finger etwas kaltes berührt. Schnell wühlst du den Gegenstand frei. Es ist der Schlüsselring - mit dem Schlüssel daran.\nZärtlich streichst du den Schmutz von dem Schlüsselkärtchen ab, doch die Handschrift deines Großvaters ist nicht mehr erkennbar. Dennoch nimmst du den Schlüssel an dich.\nDu stehst auf und verläßt den Kartoffelkeller und gehst zurück.");
		schluesselgefunden = true;
	}
	else
		textausgabe("Du gehst durch den nach Moder riechenden Gang. Als du an das ehemalige Kartoffellager deiner Großmutter kommst, mußt du feststellen, das es einfach nur leer und verdreckt ist. Seit ihrem Tod hat es wohl niemand mehr betreten. Die Kartoffeln müssen schon vor Jahren verschimmelt sein. Du wendest den Blick zur Seite. Die Klappe, durch die früher immer die Bierfässer hinabgelassen wurden, befindet sich immer noch am Ende des Ganges. Jetzt würde sie allerdings in den Wintergarten der Gaststätte führen. Da du hier nichts weiter finden kannst, beschließt du zurückzugehen.");
	ort20();
}

void ort24(void) {
// Rückwärtige Keller
	raum = 24;
	textausgabe("Du gehst an der Kellertreppe vorbei. Hinter ihr macht der Gang einen Knick nach rechts, und nach einem kurzen Durchang stehst du vor einem breiten Gang. Überall sind durch hölzerne Boxen getrennt die Kellerabteile der Mieter dieses Hauses. Der Verschlag, hinter dem deine Sachen gesichert waren, ist leer, bis auf die Erinnerungen daran in deinem Kopf. Du drehst dich um und gehst zurück.");
	ort20();
}

void ort25(void) {
// Die Treppe hinaufsteigen
	raum = 25;
	if(!buchgefunden && schluesselgefunden) {
		textausgabe("Du steigst die Treppen hinauf bis zum Dachgeschoss. Hinter der vorletzten Türe auf der rechten Seite, befindet sich das Spielzimmer, daß dein Großvater dir schenkte. Hier hatte er auf einer Spanplatte eine Eisenbahn mit Bergen und Tunneln für dich aufgebaut, auch wenn er es meistens war, der die Züge fahren ließ. Du steckst den Schlüssel, den du im Keller gefunden hast, ins Schloss - und schließt die Türe auf. Du öffnest sie - und vor dir schwingt sie nach innen. Die Eisenbahn ist immer noch fest montiert auf der Spanplatte, die auf dem rötlichen Boden liegt. Du trittst ein - und schließt die TÜre hinter dir, schließt sie sogar ab, um ungestört zu sein. Auf einem Holzstuhl findest du ein Buch. Als du es dir nimmst und umdrehst, ist es der fehlende Band aus der Bibliothek. Groß prangt der Name Agartha darauf. Du setzt dich auf den Stuhl und blätterst es durch. Immer wieder stößt du auf Passagen in dem Buch, die dein Großvater mit Bleistift unterstrichen hat. In den Passagen ist die Rede von einer Stadt, die sich in den Tiefen der Erde befinden soll. Es finden sich Referenzen darauf, das es Eingänge zu dieser Stadt geben soll - und das wenigstens einer dieser Eingänge in Tibet ist. Plötzlich flattert ein Blatt aus dem Buch. Es muß zwischen den Seiten gelegen haben. Du bückst dich und hebst es auf. Mit der krakeligen Schrift deines Großvaters steht darauf notiert: \"Münster   Sarg   Innenhof   Keller   Rheydt\"\nDu steckst den Zettel und das Buch in deinen Rucksack.");
		buchgefunden = true;
	}
	else {
		if(wuerfel(6) > 4)
			textausgabe("Du steigst die Treppen bis zur dritten Etage hinauf. Von hier aus schaust du hinüber zu dem Balkon, wo früher dein Freund Guido gelebt hat. Du erinnerst dich daran, wie ihr ein primitives Blechdosentelefon gespannt hattet, dessen eines Ende ihr mit einem Holzflitzebogen ihr hinübergeschossen hattet. In Erinnerungen versunken steigst du danach die Treppe wieder hinunter ins Erdgeschoss.");
		else
			textausgabe("Du ersteigst die Treppenstufen bis hinauf in das Dachgeschoss. Noch einmal versuchst du, ob du eine der verschlossenen Türen aufbekommst und schaust durch die Schlüssellöcher, doch erscheinen dir alle Räume leer und verlassen. Du drehst dich um und gehst die Treppenstufen wieder hinab in das Erdgeschoss.");
	}
	ort21();
}

void ort26(void) {
// Der Alte Markt
	raum = 26;
	auswahl("Von hier aus kannst du zur Waldhausener Straße (1), dem Kapuzinerplatz (2), dem Marktstieg (3), den Abteiberg hinunter (4), zur Hindenburgstraße (5) oder durch die Passage zur Turmstiege (6)", 6, ort43, ort28, ort27, ort35, ort34, ort44);
}


void ort27(void) {
	// Der Marktstieg
	auswahl("Möchtest du zur Hindenburgstraße (1), zum Alten Markt (2), zum Kapuzinerplatz (3), zur Wallstraße (4) oder zur Kaiserstraße (5)?", 5, ort34, ort26, ort28, ort45, ort29, NULL);
}

void ort28(void) {
	// Der Kapuzinerplatz
	auswahl("Von hier aus kannst du zum Alten Markt (1), zur Turmstiege (2), das Haus Zoar betreten (3), zur Kaiserstraße (4), zum Marktstieg (5)", 5, ort26, ort44, ort30, ort29, ort27, NULL);
}

void ort29(void) {
	// Die Kaiserstraße
	auswahl("Möchtest du die Straße hinab zur Blücherstraße (1), zur Wallstraße (2), zum Marktstieg (3) oder zum Kapuzinerplatz (4)?", 4, ort31, ort45, ort27, ort28, NULL, NULL);
}

void ort30(void) {
	// Haus Zoar
	raum = 30;
	auswahl("Willst du zurück auf den Kapuzinerplatz (1) oder durch das geöffnete Fenster hinabspringen und hinüber zum Beginn der Kaiserstraße laufen (2)?", 2, ort28, ort29, NULL, NULL, NULL, NULL);
}

void ort31(void) {
	// Blücherstraße
	auswahl("Willst du weiter dem Berg hinauf die Kaiserstraße entlang (1), in Richtung Kleiststraße (2) oder in die Stadtbibliothek (3)?", 3, ort29, ort50, ort32, NULL, NULL, NULL);
}

void ort32(void) {
	// Stadtbibliothek
	if(verzeichnisgelesen && agartha && !kartegefunden) {
		textausgabe("Du erkletterst hastig den Geröllhaufen, über den du in das Innere der Stadtbibliothek gelangst. Du gehst die Reihen der Apothekerschränke ab, bis du das Fach erwischst, in der Du dir einen Hinweis auf das Wort \"Agartha\" erhoffst. Als du endlich die Karte mit dem Begriff hast, stehen darauf 3 Bücher. Du nimmst dir die Karte heraus und durchstöberst die Bibliothek. Zwanzig Minuten später, als du das dritte Buch in der Hand hältst, erkennst du unter den Ausleihern des Buches deinen Großvater in der Liste. Beim Durchblättern des Buches bemerkst du, daß darin eine Karte abgebildet ist. Du liest dir die Texte der Innenumschläge durch und staunst nicht schlecht. In dem Buch ist die Rede von einer inneren Welt. Du erinnerst dich an das Buch von Jules Verne, daß dir dein Großvater geschenkt hatte. Es liest sich unglaublich, daß auch diese Theorie Vernes wohl auf einem Körnchen Wahrheit beruht. Agartha soll einer dieser Orte sein. Das Buch verstaust du kurzerhand in deinem Rucksack, schulterst ihn und verläßt auf dem schnellsten Wege die Stadtbibliothek.");
		kartegefunden = true;
		raum = 32;
		ort31();
	}
	if(!verzeichnisgelesen || !agartha) {
		textausgabe("Die Stadtbibliothek erscheint von außen betrachtet menschenleer. Ein Teil der vorderen Fassade ist eingebrochen, du kletterst über den dadurch entstandenen Geröllhaufen und gelangst in das Innere der Bibliothek. Da es keinen Strom gibt, bist du dankbar für die Unmenge an Apothekerschränken in deren ausziehbaren Schubladen es nur so von Karteikartenbeschreibungen der Bücher wimmelt. Leider aber hast du keinerlei Idee, wonach du suchen könntest. Da du nichts in der Bibliothek findest, was dir momentan helfen könnte, verläßt du sie.");
		verzeichnisgelesen = true;
		raum = 32;
		ort31();
	}
	textausgabe("Kaum daß du die Bibliothek betreten hast, glaubst du hinter den Regalen eine Gestalt wahrzunehmen.");
	ort13();
	textausgabe("Um nicht noch in weitere Scherereien hineinzustolpern, verlä8t du die Bibliothek.");
	raum = 32;
	ort31();
}

void ort33(void) {
	// Im Lichthof
	auswahl("Möchtest du die Hindenburgstraße hinab laufen (1), oder lieber bergauf (2), oder in die andere Richtung zur Kleiststraße (3)?", 3, ort51, ort34, ort50, NULL, NULL, NULL);
}

void ort34(void) {
	// Hindenburgstraße, oberer Teil
	auswahl("Von hier aus kannst du weiter die in Richtung Lichthof (1), in Richtung des Hauptbahnhofs (2), zur Wallstraße (3), in Richtung des Alten Markts (4) oder in Richtung Abteiberg (5). Du kannst es auch die Croonsallee entlang zur Kaiserstraße (6).", 6, ort33, ort51, ort45, ort26, ort35, ort29);
}

void ort35(void) {
	// Abteiberg
	raum = 35;
	auswahl("Möchtest du zum Alten Markt (1), die Hindenburgstraße hinab (2), den Vorplatz am Münster entlang (3), das alte Rathaus betreten (4)? oder zum Park am Spatzenberg (5)?", 5, ort26, ort34, ort37, ort36, ort52, NULL);
}

void ort36(void) {
	// Das Rathaus
	raum = 36;
	auswahl("Möchtest du hinaus auf den Abteiberg (1) oder den Innenhof betreten (2)?", 2, ort35, ort41, NULL, NULL, NULL, NULL);
}

void ort37(void) {
	// Der Vorplatz des Münsters
	raum = 37;
	auswahl("Willst du weiter in Richtung des Abteibergs (1), die Treppen hinab zum Geroweiher (2) oder willst du in das Gladbacher Münster hinein (3)?", 3, ort35, ort46, ort38, NULL, NULL, NULL);
}

void ort38(void) {
	// Im Hauptschiff des Münsters
	auswahl("Du kannst das Münster verlassen (1) oder den Seitengang betreten (2).", 2, ort37, ort39, NULL, NULL, NULL, NULL);
}

void ort39(void) {
	// Im Seitengang des Münsters
	auswahl("Von hier aus gelangst du in das Hauptschiff des Münsters (1), eine Türe führt in den Innenhof (2) und über eine Treppe gelangst du in die Gruft unter den Altar (3).", 3, ort38, ort41, ort40, NULL, NULL, NULL);
}

void ort40(void) {
	// Die Gruft unter dem Altar
		textausgabe("Du steigst die Treppe hinunter in die Gruft, die direkt unter dem Altar des Münsters liegt. Du erinnerst dich, wie in der dritten Klasse einmal ein Klassenausflug hierhin führte und die Person, die euch das Münster zeigte, auch an diesem Ort mit euch kam. Sie erzählte eine krude Sage, von Soldaten, einem Spiel und dem Abdruck der Hand des Teufels auf dem Sarg noch einmal genauer an.");
	if(kartegefunden && !sargverschoben)
		if(janeinfrage("Willst du dir den Inhalt des Sarges näher ansehen (j/n)?")) {
			textausgabe("Mit aller Kraft gelingt es dir, wenn auch nur sehr langsam, den Sargdeckel zu verschieben - allerdings nicht sehr weit. Doch was du bereits sehen kannst, reicht dir aus: der Sarg ist vollkommen leer. Du blickst dich noch kurz um, und überlegst, ob du auch die Deckel der anderen Särge verschieben sollst. Angesichts des unnützen Kraftaufwands und der Tatsache, das bereits in diesem Sarg nichts drin ist, bist du der festen Überzeugung, das es sich bei den anderen ebenso verhält. Du wendest dich ab und gehst die Treppe hinauf.");
			sargverschoben = true;
			raum = 40;
			ort39();
		}
	if(sargverschoben) {
		if(janeinfrage("Willst du den Sargdeckel wieder in seine ursprüngliche Position schieben (j/n)?")) {
			textausgabe("Mit aller Kraft gelingt es dir, wenn auch nur sehr langsam, den Sargdeckel wieder zu schließen. Als du es endlich geschafft hast, könntest du schwören, ein dumpfes, leises Geräusch von irgendwoher vernommen zu haben.");
			sargverschoben = false;
		}
	}
	textausgabe("Da die Gruft ansonsten leer ist, verläßt du sie und steigst die Treppe hinauf.");
	raum = 40;
	ort39();
}

void ort41(void) {
	// Der Innenhof
	auswahl("Durch eine Türe gelangst du in das Münster (1), ein weiterer Durchgang führt in das alte Rathaus (2). Eine Treppe in der Mitte des Innenhofes führt zu einer Stahltüre hinab (3).", 3, ort39, ort36, ort42, NULL, NULL, NULL);
}

void ort42(void) {
	// Der Keller unter dem Innenhof
	if(sargverschoben) {
		textausgabe("Du steigst die Stufen in der Mitte des Innenhofes hinab und öffnest die Stahltüre. Als du in den Raum hineinblickst, erkennst du, das sich ein Spalt in einer Wand geöffnet hat. Du gehst in den Raum hiein und bemerkst beim näherkommen, das der Spalt tatsächlich eher ein richtiger Durchgang ist - und das dahinter ein Gang liegt.");
		raum = 42;
		auswahl("Möchtest du den Gang betreten (1) oder willst du den Raum verlassen und die Treppe hinauf in den Innenhof gehen (2)?", 2, ort53, ort41, NULL, NULL, NULL, NULL);
	}
	textausgabe("Du steigst die Stufen in der Mitte des Innenhofes hinab und öffnest die Plastikklinge an der Stahltüre. Die Türe öffnet sich problemlos. Der Innenraum riecht muffig, die Wände sind alt und nicht überall eben. Der Raum wird offensichtlich für nichts mehr benutzt. Da es hier nichts weiter zu sehen gibt, drehst du dich um, verläßt den Raum und steigst die Treppe hinauf in den Innenhof.");
	raum = 42;
	ort41();
}

void ort43(void) {
	// Die Waldhausener Straße
	raum = 43;
	auswahl("Die Straße führt hinauf zum Alten Markt (1), neben dem Dicken Turm verläuft die Turmstiege (2) und ein weiterer Weg führt durch den kleinen Grünbereich des Spatzenberges (3)", 3, ort26, ort44, ort52, NULL, NULL, NULL);
}

void ort44(void) {
	// Die Turmstiege
	auswahl("Von hier aus hast du die Möglichkeit zur Waldhausener Straße zu gelangen (1), durch die Passage zum Alten Markt zu gehen (2) oder zum Kapuzinerplatz(3)", 3, ort43, ort26, ort28, NULL, NULL, NULL);
}

void ort45(void) {
	// Die Wallstraße
	auswahl("Du kannst von hier aus zur Hindenburgstraße (1), die Kaiserstraße hinab (2), den Marktstieg entlang (3) oder am Haus Zoar vorbei zum Kapuzinerplatz (4)", 4, ort34, ort29, ort27, ort28, NULL, NULL);
}

void ort46(void) {
	// Der Geroweiher
	if(raum == 55)
		textausgabe("Mit lange Zügen tauchst du weiter durch das eiskalte Wasser. Deine Lungenflügel leeren sich und beginnen langsam zu brennen. Du weißt, daß du den Rückweg nicht mehr schaffen würdest, also schwimmst du was das Zeug hält. Plötzlich bemerkst du über dir ein Licht - du hältst drauf zu - und durchbrichst die Wasseroberfläche. Luft strömt in deine gequälten Lungenflügel. Du ruderst mit den Armen schaffst es, nicht wieder einzutauchen und ruderst halb benommen zum Ufer, an dem du liegenbleibst. Du mußt etwas verschnaufen. Dann, als du wieder klarer denken kannst, nimmst du deine Umgebung wahr - und erkennst, das du am Ufer des Geroweihers liegst, unter den schützenden Ästen der alten Trauerweide, an denen ihr als Kind immer mit Tarzanschrei ins Wasser geschwunden seid. Du bleibst weiter einfach sitzen und schwelgst in Erinnerungen, bis du dir die aktuelle Gefahr wieder vor Augen führst und dich erhebst.");
	else
		textausgabe("Der Geroweiher.\nEin kleiner Ort der Ruhe, an dem ein Spielplatz in deiner Jugend für ein gewisses Training deiner Muskeln sorgte, der aber auch der Austragungsort so mancher Keilerei war. Im Zentrum steht ein Stück alter Stadtmauer und wenn man hochguckt, sieht man auf dem Hügel das Münster. Der Weiher ist vielleicht gerade einmal zwei, höchstens drei Meter weit. Wenn du dich recht entsinnst, hatte eure Grundschullehrerin euch früher erzählt, der Geroweiher würde durch den Gladbach gespeist werden. Aber egal wie weit du auch zurückdenkst, den Gladbach hast du hier noch nirgendwo fließen sehen. Vielleicht, ist er ja nur noch eine Erinnerung, an alte Zeiten.");
	raum = 46;
	auswahl("Du kannst den Spatzenberg hinauf (1), oder den Abteiberg (2) oder die Treppen zum Münstervorplatz nehmen (3)", 3, ort52, ort35, ort37, NULL, NULL, NULL);
}

void ort47(void) {
	// Die Bismarkstraße
	auswahl("Der Weg ist soweit sicher die Hindenburgstraße hinauf (1) oder weiter hinab bis zum Vorplatz des Hauptbahnhofs (2)", 2, ort51, ort48, NULL, NULL, NULL, NULL);
}

void ort48(void) {
	// Der Bahnhofsvorplatz
	raum = 48;
	auswahl("Du kannst entweder die Hindenburgstraße entlang in Richtung Alter Markt (1) oder in den Hauptbahnhof hinein (2)", 2, ort47, ort49, NULL, NULL, NULL, NULL);
}

void ort49(void) {
	// Der Hauptbahnhof
	ort48();
}

void ort50(void) {
	// Die Kleiststraße
	raum = 50;
	auswahl("Du kannst am Adenauerplatz vorbei zur Blücherstraße (1) oder Richtung Hindenburgstraße in den Lichthof (2)", 2, ort31, ort33, NULL, NULL, NULL, NULL);
}

void ort51(void) {
	// Mittelteil der Hindenburgstraße
	raum = 51;
	auswahl("Von hier aus kannst du der Hindenburgstraße bergauf folgen (1) oder in die Gegenrichtung auf den Hauptbahnhof zu (2), oder Richtung Kleiststraße durch den Lichthof (3)", 3, ort34, ort47, ort33, NULL, NULL, NULL);
}

void ort52(void) {
	// Der Spatzenberg
	raum = 52;
	auswahl("Du kannst von hier aus zum Abteiberg hinaufgehen (1), hinab zum Geroweiher (2) oder hinüber zur Waldhausener Straße (3)", 3, ort35, ort46, ort43, NULL, NULL, NULL);
}

void ort53(void) {
	// Der Eingang zum Labyrinth
	if(raum == 42)
		textausgabe("Du betrittst den Gang. Er ist alt, die Luft riecht modrig, abgestanden. Aus deinem Rucksack hast du deine Taschenlampe und deinen alten Marschkompass aus der Bundeswehrzeit hervorgeholt. Du leuchtest die Wände ab und gehst langsam voran. Plötzlich hörst du hinter dir, wie sich der Spalt schließt. Du drehst dich noch um - aber es ist zu spät. Es scheint, als wäre dir der Rückweg versperrt. Hilflos zuckst du mit den Schultern. Wenn du richtig liegst, dann kannte dein Großvater diesen Ort - und er hätte dich niemals in eine Falle laufen lassen. So schreitest du weiter den Gang voran.");
	else
		textausgabe("Ganz im Westen ist der Gang eine Sackgasse. Du weißt, daß dort eine TÜre ist, aber sie ist zu perfekt eingefasst - und du findest nichts, um sie zu öffnen. So gibst du nach einer Weile des Suchens auf und folgst dem Gang nach Osten.");
	ort54();
}

void ort54(void) {
	// Der Raum mit der versteckten Rutschbahn
	if(objekt[rucksack] > 0)
		textausgabe("Du betrittst einen mehr oder weniger rechteckigen Raum. Es sind verschiedene, unleserliche Kritzeleien und Schriftzeichen an den Wänden zu sehen, jedoch nichts, was du wirklich entziffern könntest.");
	else {
		textausgabe("Du tastest dich durch den stockfinsteren Raum, bis du glaubst den Durchgang nach Norden gefunden zu haben und folgst diesem weiter.");
		ort55();
	}
	raum = 54;
	if(durchganggeoeffnet && (objekt[rucksack] > 0)) {
		textausgabe("In der Südostecke des Raumes hat sich ein Stück des Bodens verschoben. Dort ist ein Loch im Boden, in dem eine Rutsche ist. Nachdem du sie dir näher angesehen und betastet hast, kommst du zu dem Ergebnis, daß das Metall zu glatt ist - um im Falle eines Falles dort wieder hochklettern zu können - hingegen wäre es wohl eine Leichtigkeit - hinunterzurutschen.");
		auswahl("Es führen zwei Wege aus dem Raum heraus, der eine führt nach Westen (1), der andere nach Norden (2). Du könntest einen von ihnen nehmen - oder aber eine ungewisse Rutschpartie wagen (3).", 3, ort53, ort55, ort56, NULL, NULL, NULL);
	}
	else
		auswahl("Es führen zwei Wege aus dem Raum heraus, der eine führt nach Westen (1), der andere nach Norden (2). Welchen möchtest du einschlagen?", 2, ort53, ort55, NULL, NULL, NULL, NULL);
}

void ort55(void) {
	// Der Durchgang zum Geroweiher
	raum = 55;
	if(!objekt[rucksack]) {
		textausgabe("Du tastest dich durch den Raum, bis du schließlich auf deinen Rucksack stößt.");
		if(janeinfrage("Möchtest du dich wieder ankleiden und deinen Rucksack, deine Taschenlampe und alles andere an dich nehmen (j/n)?"))
			objekt[rucksack] = 1;
	}
	else {
		textausgabe("Der Raum, in den du gelangst, gleicht eher einer Höhle mit Natursteinen. Es gibt nur einen Weg hinaus, der im Süden, durch den du hineingekommen bist. An seiner Westwand ist ein großes, dunkles Wasserloch.");
		if(janeinfrage("Möchtest du deine Kleidung und deinen Rucksack samt Taschenlampe ablegen und in das dunkle Wasserloch tauchen (j/n)?")) {
			objekt[rucksack] = 0;
			textausgabe("Vorsichtig steckst du einen Zeh in das Wasser. Es ist eiskalt. Langsam, ganz langsam steigst du immer tiefer rein. Der Boden sinkt soweit ab, das du ab einer gewissen Grenze nicht mehr stehen kannst. Es ist jetzt stockfinster im Raum. Schräg in der Tiefe glaubst du ein Licht wahrzunehmen. Du holst noch einmal tief Luft - und tauchst tief in das Wasser ein. Du tauchst durch eine Art natürlicher Tunnel, dessen Wände sich allerdings glatt anfühlen. An einer Stelle des Tunnels stößt du gegen einen Widerstand, aber der verflüchtigt sich sofort. Vermutlich war es nur ein verirrter Fisch.");
			durchganggeoeffnet = true;
			ort46();
		}
	}
	textausgabe("Da der Raum außer dem Wasserloch nichts zu bieten hat, verläßt du ihn und gehst zurück nach Süden.");
	ort54();
}

void ort56(void) {
	// Die Rutschbahn
}

void ort57(void) {
	raum = 57;
}

void ort58(void) {
	raum = 58;
}

void ort59(void) {
}

void ort60(void) {
}

void ort61(void) {
}

void ort62(void) {
	raum = 62;
}

void ort63(void) {
	raum = 63;
}

void ort64(void) {
}

void ort65(void) {
}

void ort66(void) {
	raum = 66;
}

void ort67(void) {
	raum = 67;
}

void ort68(void) {
}

void ort69(void) {
}

void ort70(void) {
}

void ort71(void) {
}

void ort72(void) {
}

void ort73(void) {
}

void ort74(void) {
}

void ort75(void) {
}

void ort76(void) {
}

void ort77(void) {
	raum = 77;
}

void ort78(void) {
	raum = 78;
}

void ort79(void) {
	raum = 79;
}

void ort80(void) {
	raum = 80;
}

void ort81(void) {
}

void ort82(void) {
}

void ort83(void) {
}

void ort84(void) {
}

void ort85(void) {
	raum = 85;
}

void ort86(void) {
}

void ort87(void) {
}

void ort88(void) {
	raum = 88;
}

void ort89(void) {
}

void ort90(void) {
}

void ort91(void) {
}

void ort92(void) {
}

void ort93(void) {
}

void ort94(void) {
	raum = 94;
}

void ort95(void) {
}

void ort96(void) {
}

void ort97(void) {
	raum = 97;
}

void ort98(void) {
}

void ort99(void) {
}

// -----------
// --- 100 ---
// -----------

void ort100(void) {
// Du stimmst Elke zu und ihr verlaßt das Parkhaus
	raum = 100;
	textausgabe("Ihr verlaßt das Auto. Elke fährt den Wagen die Stepgesstraße hinunter und hält vor der Ampel. Sobald Grün kommt, überquert sie die Kreuzung und fährt den Berliner Platz entlang. Du schaust hinüber zum Kaiserbad, wo du als kleines Kind schwimmen gelernt hast.\n\"Schaltest du mal das Radio an?\" bittet sie dich und du wendest dich der Mittelkonsole zu. Kurz darauf ist das Digitalradio an. \"Mit Radio 90,1 wird das nichts. Der Sender hat wohl keinen Saft.\" kommentierst du den vorausgewälten Sender und probierst einen Senderplatz nach dem anderen. Schließlich empfängt das Radio ein Signal auf der Deutschen Welle.\n\"... wird im Westen der Republik von verheerenden Unwettern berichtet, die auch im Venloer Raum bereits für erhebliche Zerstörung gesorgt haben sollen. Fast die ganze Niederlande und der Niederrhein verfügen über keinerlei Strom mehr. Auch die offiziellen Behördennetzwerke in dem Gebiet können nicht durch die Bundesregierung erreicht werden. Derweil hat der Kanzler eine Mobilmachung der Bundeswehr angeordnet, die eine Versorgung des Gebietes erreichen soll. Aufnahmen von Wettersatelliten zeigen in der Region nichts weiter als eine ausnehmend großen Masse an dichten dunklen Wolken....\"\nIm nächsten Augenblick hatte das Radio auch die Deutsche Welle verloren. Der Wagen rollte derweil die Aachener Straße in Richtung Rheindahlen, Wegberg, Erkelenz entlang. Sie kamen am neuen Bundesliga-Stadion vorbei, doch die Sicht wurde jetzt weiter, offener. Überall im Westen näherte sich eine riesige, bedrohliche Wolkenwand. Selbst im Süden konnte man jetzt sehen, daß sie auch dort waren. Im Osten hingegen kam nun ein kleiner Spalt hellblauen Himmels zum Vorschein. Elke fuhr mit dem Auto auf einen Wendeplatz und hielt den Wagen an.\n\"Was denkst du, wohin sollen wir jetzt fahren?\"");
	auswahl("Wenn du weiter nach Süden willst (1), nach Westen, wo die Wolken noch viel näher und finsterer sind (2), Osten, wo ein Streifen blauer Himmel jetzt sichtbar wird (3), oder zurück nach Mönchengladbach, wo die Turmmonster sind, du dich aber auskennst (4).", 4, ort106, ort107, ort108, ort109, NULL, NULL);
}

void ort101(void) {
// Du hast ein mulmiges Gefühl und versuchtst Elke vom rausfahren abzuhalten
	raum = 101;
	if(tripodgesehen == true) {
		textausgabe("Du erzählst Elke alles, was du gesehen hast, und beschreibst ihr verbal dieses turmgroße Etwas, das sich wie auf Stelzen zu bewegen schien und von dem aus Lichtbögen die Umgebung zerstörten.\nElke sieht dich irritiert an und fragt schließlich, wieso du ihr das nicht schon früher gesagt hättest? Schließlich bricht sie in schallendes Gelächter aus. Es vergeht eine Minute, bis sie sich wieder unter Kontrolle hat.\n\"Ich denke, du bist ein echt lieber Kerl, aber auch ein ziemlicher Phantast, hm?\"\nSie wirft den Motor an. Du glaubst deiner Erinnerung auch nicht wirklich - und irgendwie scheint es sowieso egal zu sein. Im Moment ist einzig und allein am wichtigsten, einmal etwas über die aktuelle Lage in Erfahrung zu bringen. Langsam rollt der Wagen durch das Parkhaus.");
	}
	else {
		textausgabe("Du erzählst ihr, das du ein ungutes Gefühl hast, ob es nun an dem schlechten Wetter, den Sirenen oder was auch immer liegt, kannst du nicht sagen, aber du bringst klar zum Ausdruck, daß du kein gutes Gefühl dabei hast, in einem Auto auf die Straßen zu fahren, nicht jetzt, nicht bei diesem Wetter, der Situation - und erst recht nicht, wenn es nach deinem Bauchgefühl geht. Aber Elkes Einwände sind besser, immerhind könntet ihr ein wenig erfahren, wieso die ganze Stadt einen Stromausfall erfahren hat. Du zuckst noch einmal mit den Schultern und willigst ein, schnallst dich an und schon geht es aus der Parklücke heraus und ihr rollt durch das dunkle Parkhaus.");
	}
	textausgabe("Plötzlich gibt es so etwas wie einen kurzen, heftigen Erdstoß. Von einigen wenigen Autos schaltet sich die Diebstahlwarnanlage and und eröffnet eine neue Kakophonie innerhalb des Parkhauses. Jetz wäre dir wirklich nichts lieber, als endlich hier heraus zu sein. Ihr nähert euch der Ausfahrt. Elke schiebt ihre Karte in den alten Scheidt&Bachmann, dann öffnet sich die Schranke. Ihr fahrt langsam an - und verlaßt das Parkhaus. Elke will gerade abfahren, als zum zweiten Mal ein Erdstoß erfolgt.\n\"Gib bitte Gas!\" sagst du zu ihr, mit einem mummeligen Gefühl im Magen. Elke nickt und fährt los.");
	versuchedeinglueck(ort103, ort102);
}

void ort102(void) {
// Kein Glück - es hat Elke erwischt
	raum = 102;
	textausgabe("Mit einem fürchterlichem Geräusch von schneidendem Metall, einer unglaublichen Erschütterung und einem unbeschreiblichem, donnernden Geräusch, wird ein Teil eures Autos zerteten, durchgeteilt, vernichtet. Ein riesiger Metallfuß, wie du ihn von AT-AT aus dem Kino kennst, hat einen Teil des Motorblocks und die Fahrerseite zerquetscht. Du selber bist auch etwas eingequetscht. Dein Herz rast vor Angst und entsetzen. Neben dir, daß weißt du instinktiv, hat gerade Elke ihren Tod gefunden. Obwohl du sie kaum kanntest, tut dir das in der Seele weh, du weinst - versuchst aber gleichzeitig keinen Mucks zu machen - um nicht das gleiche Schicksal zu erleiden. In deiner verzweifelten Lage klappst du den Sitz langsam nach hinten. Du schaffst es, die Befestigung des Rücksitzes zu lösen - und kannst so deinen Rucksack aus dem Kofferraum holen. Du holst zusätzlich noch die Beutel die du erreichen kannst und stopfst dir deinen Rucksack damit voll. Du versuchst nicht zur Seite zu schauen, dir ein Bild von Elke zu machen.");
	auswahl("Willst du dich weiter, so leise wie möglich verhalten (1) oder willst du versuchen, aus dem Auto herauszukommen - und zu fliehen. Immerhin könnte der riesige Fuß ja noch ein zweites Mal auf das Auto herabsausen (2)?", 2, ort104, ort105, NULL, NULL, NULL, NULL);
}

void ort103(void) {
// Glück gehabt
	raum = 103;
	textausgabe("Keine zwei Meter von eurem Auto entfernt, knallt ein riesiger Metallfuß mit ohrenbetäubendem Lärm und unter einer kurzen, heftigen Erschütterung, auf den Boden. Ihr werdet beide blass - doch sofort brüllst du Elke an: \"Fahr! Fahr! Fahr!\"\nSie löst sich aus ihrer Schockstarre und tritt das Gaspedal bis zum Anschlag durch. Mit einem Satz rollt ihr die Stepgesstraße hinab. Ohne auf die Ampelanlage zu achten, schießt ihr über die Kreuzung, am Kaiserbad vorbei in Richtung Korschenbroicher Straße, mit quietschenden Reifen biegt ihr in die Fliethstraße ein, während Elke immer weiter beschleunigt. Und du hast nichts dagegen einzuwenden, denn auch du willst nichts weiter, als weg. Ihr rast am Geroweiher vorbei, als du brüllst, sie soll nach Süden abbiegen. So rast ihr nahezu unaufhalsam über die Kreuzung und dann die Aachener Straße in entlang in Richtung Rheindalen. Für den Moment überlegst du ob ihr auf die Autobahn sollt, aber von dort aus kann man gar nicht mehr abbiegen.\n\"Nicht auf die Autobahn\", sagst du zu ihr, \"da säßen wir in der Falle - und unser Weg wäre vorhersehbar. Als ihr über die Brücke fahrt, die über die Autobahn führt, siehst du, wie klug eure Entscheidung war. Ein weiteres dieser Turmmonster steht mitten auf der Autobahn, einen Berg aus Blechkadavern zu seinen Füßen.\n\"Wohin fahren wir jetzt?\" will sie wissen, während ihr jetzt der Straße weiter in Richtung Wegberg und Erkelenz folgt.\n\"Erst mal weiter in Richtung Süden, an dieser Unwetterwolke und den Metalltürmen vorbei. Da entdeckt ihr weit im Süden, das die Wolke sich auch dort bereits findet. Elke fährt auf den nächsten Wendeplatz und hält den Wagen an. \"Bist du dir sicher, das wir weiter nach Süden fahren wollen?\"");
	auswahl("Wenn du weiter nach Süden willst (1), nach Westen, wo die Wolken noch viel näher und finsterer sind (2), Osten, wo ein Streifen blauer Himmel jetzt sichtbar wird (3), oder zurück nach Mönchengladbach, wo die Turmmonster sind, du dich aber auskennst (4).", 4, ort106, ort107, ort108, ort109, NULL, NULL);
}

void ort104(void) {
// Im Auto bleiben
	textausgabe("Du verhältst dich muskmäuschen still. Den Rucksack hast du auf dem Schoß. Du atmest nur gan flach, schluchzt aber auch immer wieder. Du bedauerst deine eigene verzweifelte Lage noch viel mehr, als den Tod der freundlichen Frau, die hier, nur wenige Zentimeter entfernt von dir, unter einem tonnenschweren Stahlfuß zerquetscht wurde. Plötzlich hörst rechts neben dir ein Geräusch. Erschreckt siehst du zur Seite - und starrst in die Mündung eines Automatikgewehres.\nBefriedigt grunzt das Wesen, das gerade seine Waffe in deinem Kopf entladen hat, dann wendet es sich vom Anblick deines Kadavers ab und sucht nach weiteren \"Ausreißern\".");
	exit(EXIT_SUCCESS);
}

void ort105(void) {
// Flucht
	raum = 105;
}

void ort106(void) {
// Süden
	raum = 106;
}

void ort107(void) {
// Westen
	raum = 107;
}

void ort108(void) {
// Osten
	raum = 108;
}

void ort109(void) {
// Norden
	raum = 109;
}

void ort110(void) {
}

void ort111(void) {
}

void ort112(void) {
}

void ort113(void) {
}

void ort114(void) {
}

void ort115(void) {
}

void ort116(void) {
}

void ort117(void) {
}

void ort118(void) {
}

void ort119(void) {
}

void ort120(void) {
}

void ort121(void) {
}

void ort122(void) {
}

void ort123(void) {
}

void ort124(void) {
}

void ort125(void) {
}

void ort126(void) {
}

void ort127(void) {
}

void ort128(void) {
}

void ort129(void) {
}

void ort130(void) {
}

void ort131(void) {
}

void ort132(void) {
}

void ort133(void) {
}

void ort134(void) {
}

void ort135(void) {
}

void ort136(void) {
}

void ort137(void) {
}

void ort138(void) {
}

void ort139(void) {
}

void ort140(void) {
}

void ort141(void) {
}

void ort142(void) {
}

void ort143(void) {
}

void ort144(void) {
}

void ort145(void) {
}

void ort146(void) {
}

void ort147(void) {
}

void ort148(void) {
}

void ort149(void) {
}

void ort150(void) {
}

void ort151(void) {
}

void ort152(void) {
}

void ort153(void) {
}

void ort154(void) {
}

void ort155(void) {
}

void ort156(void) {
}

void ort157(void) {
}

void ort158(void) {
}

void ort159(void) {
}

void ort160(void) {
}

void ort161(void) {
}

void ort162(void) {
}

void ort163(void) {
}

void ort164(void) {
}

void ort165(void) {
}

void ort166(void) {
}

void ort167(void) {
}

void ort168(void) {
}

void ort169(void) {
}

void ort170(void) {
}

void ort171(void) {
}

void ort172(void) {
}

void ort173(void) {
}

void ort174(void) {
}

void ort175(void) {
}

void ort176(void) {
}

void ort177(void) {
}

void ort178(void) {
}

void ort179(void) {
}

void ort180(void) {
}

void ort181(void) {
}

void ort182(void) {
}

void ort183(void) {
}

void ort184(void) {
}

void ort185(void) {
}

void ort186(void) {
}

void ort187(void) {
}

void ort188(void) {
}

void ort189(void) {
}

void ort190(void) {
}

void ort191(void) {
}

void ort192(void) {
}

void ort193(void) {
}

void ort194(void) {
}

void ort195(void) {
}

void ort196(void) {
}

void ort197(void) {
}

void ort198(void) {
}

void ort199(void) {
}

// -----------
// --- 200 ---
// -----------

void ort200(void) {
}

void ort201(void) {
}

void ort202(void) {
}

void ort203(void) {
}

void ort204(void) {
}

void ort205(void) {
}

void ort206(void) {
}

void ort207(void) {
}

void ort208(void) {
}

void ort209(void) {
}

void ort210(void) {
}

void ort211(void) {
}

void ort212(void) {
}

void ort213(void) {
}

void ort214(void) {
}

void ort215(void) {
}

void ort216(void) {
}

void ort217(void) {
}

void ort218(void) {
}

void ort219(void) {
}

void ort220(void) {
}

void ort221(void) {
}

void ort222(void) {
}

void ort223(void) {
}

void ort224(void) {
}

void ort225(void) {
}

void ort226(void) {
}

void ort227(void) {
}

void ort228(void) {
}

void ort229(void) {
}

void ort230(void) {
}

void ort231(void) {
}

void ort232(void) {
}

void ort233(void) {
}

void ort234(void) {
}

void ort235(void) {
}

void ort236(void) {
}

void ort237(void) {
}

void ort238(void) {
}

void ort239(void) {
}

void ort240(void) {
}

void ort241(void) {
}

void ort242(void) {
}

void ort243(void) {
}

void ort244(void) {
}

void ort245(void) {
}

void ort246(void) {
}

void ort247(void) {
}

void ort248(void) {
}

void ort249(void) {
}

void ort250(void) {
}

void ort251(void) {
}

void ort252(void) {
}

void ort253(void) {
}

void ort254(void) {
}

void ort255(void) {
}

void ort256(void) {
}

void ort257(void) {
}

void ort258(void) {
}

void ort259(void) {
}

void ort260(void) {
}

void ort261(void) {
}

void ort262(void) {
}

void ort263(void) {
}

void ort264(void) {
}

void ort265(void) {
}

void ort266(void) {
}

void ort267(void) {
}

void ort268(void) {
}

void ort269(void) {
}

void ort270(void) {
}

void ort271(void) {
}

void ort272(void) {
}

void ort273(void) {
}

void ort274(void) {
}

void ort275(void) {
}

void ort276(void) {
}

void ort277(void) {
}

void ort278(void) {
}

void ort279(void) {
}

void ort280(void) {
}

void ort281(void) {
}

void ort282(void) {
}

void ort283(void) {
}

void ort284(void) {
}

void ort285(void) {
}

void ort286(void) {
}

void ort287(void) {
}

void ort288(void) {
}

void ort289(void) {
}

void ort290(void) {
}

void ort291(void) {
}

void ort292(void) {
}

void ort293(void) {
}

void ort294(void) {
}

void ort295(void) {
}

void ort296(void) {
}

void ort297(void) {
}

void ort298(void) {
}

void ort299(void) {
}

// -----------
// --- 300 ---
// -----------

void ort300(void) {
}

void ort301(void) {
}

void ort302(void) {
}

void ort303(void) {
}

void ort304(void) {
}

void ort305(void) {
}

void ort306(void) {
}

void ort307(void) {
}

void ort308(void) {
}

void ort309(void) {
}

void ort310(void) {
}

void ort311(void) {
}

void ort312(void) {
}

void ort313(void) {
}

void ort314(void) {
}

void ort315(void) {
}

void ort316(void) {
}

void ort317(void) {
}

void ort318(void) {
}

void ort319(void) {
}

void ort320(void) {
}

void ort321(void) {
}

void ort322(void) {
}

void ort323(void) {
}

void ort324(void) {
}

void ort325(void) {
}

void ort326(void) {
}

void ort327(void) {
}

void ort328(void) {
}

void ort329(void) {
}

void ort330(void) {
}

void ort331(void) {
}

void ort332(void) {
}

void ort333(void) {
}

void ort334(void) {
}

void ort335(void) {
}

void ort336(void) {
}

void ort337(void) {
}

void ort338(void) {
}

void ort339(void) {
}

void ort340(void) {
}

void ort341(void) {
}

void ort342(void) {
}

void ort343(void) {
}

void ort344(void) {
}

void ort345(void) {
}

void ort346(void) {
}

void ort347(void) {
}

void ort348(void) {
}

void ort349(void) {
}

void ort350(void) {
}

void ort351(void) {
}

void ort352(void) {
}

void ort353(void) {
}

void ort354(void) {
}

void ort355(void) {
}

void ort356(void) {
}

void ort357(void) {
}

void ort358(void) {
}

void ort359(void) {
}

void ort360(void) {
}

void ort361(void) {
}

void ort362(void) {
}

void ort363(void) {
}

void ort364(void) {
}

void ort365(void) {
}

void ort366(void) {
}

void ort367(void) {
}

void ort368(void) {
}

void ort369(void) {
}

void ort370(void) {
}

void ort371(void) {
}

void ort372(void) {
}

void ort373(void) {
}

void ort374(void) {
}

void ort375(void) {
}

void ort376(void) {
}

void ort377(void) {
}

void ort378(void) {
}

void ort379(void) {
}

void ort380(void) {
}

void ort381(void) {
}

void ort382(void) {
}

void ort383(void) {
}

void ort384(void) {
}

void ort385(void) {
}

void ort386(void) {
}

void ort387(void) {
}

void ort388(void) {
}

void ort389(void) {
}

void ort390(void) {
}

void ort391(void) {
}

void ort392(void) {
}

void ort393(void) {
}

void ort394(void) {
}

void ort395(void) {
}

void ort396(void) {
}

void ort397(void) {
}

void ort398(void) {
}

void ort399(void) {
}

void ort400(void) {
}

// ----------------
// Die Hauptroutine
// ----------------
int main(void) {
	time_t jetzt;
	bool spielerlebt = true;

	// Zufallszahlen initialisieren
	jetzt = time((time_t *) NULL);
	srand((unsigned) jetzt);

	// Umgebungsvariable setzen
	setlocale(LC_ALL, "");

	initscr(); // beginne ncurses
	atexit(quit);
	keypad(stdscr, true); // Keymapping aktivieren
	cbreak(); // kein Warten bei der Eingabe auf ENTER
	echo(); // Cursort-Echo
	scrollok(stdscr, true); // Automatisches Scrollen aktivieren
	start_color(); // Beginne mit Farben
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	clear(); // Bildschirm löschen
	curs_set(0);
	// atexit(quit); // Routine, die bei der Beendung ausgeführt wird

	color_set(2, 0); // Blaue Schrift auf schwarzem Hintergrund
	textausgabe("--------------------------");
	textausgabe("Steampunk FFR - Der Anfang");
	textausgabe("--------------------------");
	color_set(0, 0);
	textausgabe("Ein \"Das-ist-dein-Abenteuer\" Roman\n");
	color_set(2, 0);
	textausgabe("Nach einer Geschichte von Sascha Karl (Kochs) Biermanns\n");
	color_set(0, 0);
	if(janeinfrage("Möchtest du ein gespeichertes Spiel fortführen (j/n)?"))
		laden();
	intro();
	vorwort();
	ort1();
}

// Implementation x-seitiger Würfel
int wuerfel(unsigned int maximalzahl) {
	// aus z.B. 0..5 wird 1..6
	return (rand() % (maximalzahl - 1)) + 1;
}

// Implementation Teste dein Glück
bool tdg(charakter_t *figur) {
	if(figur->glueck <= 0)

		return false; // Da kann man nur noch Pech haben!
	if((wuerfel(6) + wuerfel(6)) <= figur->glueck--)
		return true; // Glück gehabt
	return false; // Pech gehabt
}

// Implementation: Kampfrunde
bool kampfrunde(charakter_t *angreifer, charakter_t *verteidiger, void (*fluchtpunkt)()) {
	int testgewandheit[2];
	bool glueckstest = false;
	bool wirklichglueck;
	int schildbonus = 0;

	testgewandheit[0] = angreifer->gewandheit + wuerfel(6) + wuerfel(6) + angriffsbonus + paralysiert;
	testgewandheit[1] = verteidiger->gewandheit + wuerfel(6) + wuerfel(6);
	if(unsichtbar) // Falls du unsichtbar bist
		testgewandheit[0] += 2;
	if(testgewandheit[0] == testgewandheit[1])
		return true; // Unentschieden, das heißt - nicht getroffen
	momentane_werte(angreifer);
	momentane_werte(verteidiger);
	// Möchte der Spieler fliehen?
	if(fluchtpunkt != NULL)
		if(janeinfrage("Möchtest du fliehen (j/n)? "))
			flucht(fluchtpunkt);
	// Möchte der Spieler sein Glück versuchen?
	if(angreifer->glueck > 0) {
		if(janeinfrage("Möchtest du dein Glück testen (j/n)? ")) {
			glueckstest = true;
			wirklichglueck = tdg(angreifer);
		}
	}
	// Die Auswertung des Kampfes
	if(testgewandheit[0] > testgewandheit[1]) { // Der Spieler geht als Sieger hervor
		if((nursilbertrifft == true) && (silberwaffe == false)) // Nur falls das Monster imun ist gegen Silber
			return true; // Kein Treffer abbekommen
		if(unsichtbar) // Unsichtbare treffen besser
			verteidiger->staerke -= 2;
		if(((objekt[gewehr] > 0) || (objekt[pistole] > 0)) && (objekt[patrone] > 0)) { // Schusswaffe = + 1 Punkt, solange Patrone vorhanden
			verteidiger->staerke -= 1;
			objekt[patrone] -= 1;
		}
		if(glueckstest) {
			if(wirklichglueck)
				verteidiger->staerke -= 4;
			else
				verteidiger->staerke -= 1;
			return true; // Kein Treffer abbekommen
		}
		verteidiger->staerke -= 2;
		return true; // Kein Treffer abbekommen
	}
	else if (testgewandheit[0] < testgewandheit[1]) { // Der Spieler verliert die Runde
		if((objekt[schild] >= 0) && (wuerfel(6) == 6)) // Der Schild fängt den Schlag ab
			schildbonus = 1;
		if(unsichtbar) // Der Unsichtbarkeitsbonus
			switch(wuerfel(6)) {
				case 2:
				case 4:
					angreifer->staerke += 1;
					break;
				case 6:
					return true; // Kein Treffer erhalten, dank Unsichtbarkeit
				default:
					break;
			}
		if(glueckstest) {
			if(wirklichglueck)
				angreifer->staerke -= 1 - schildbonus;
			else
				angreifer->staerke -= 4 - schildbonus;
			return false; // Treffer abbekommen
		}
		angreifer->staerke -= 2 - schildbonus;
		return false; // Treffer abbekommen
	}
	return true; // Unentschieden, das heißt - nicht getroffen
}

// Implementation: Kampf
bool kampf(charakter_t *spieler, charakter_t *gegner, int anzahl, bool trefferverboten, void (*fluchtpunkt)()) {
	int gesamtstaerke = 0;
	bool keintreffererhalten = true;

	for(int i=0; i < anzahl; i++)
		gesamtstaerke += gegner[i].staerke;
	while((spieler->staerke > 0) && (gesamtstaerke > 0)) {
		for(int i=0; i < anzahl; i++)
			if(spieler->staerke > 0) {
				if(gegner[i].staerke > 0)
					keintreffererhalten = kampfrunde(spieler, &gegner[i], fluchtpunkt);
				if(trefferverboten)
					if(!keintreffererhalten)
						return false;
			}
			else
				return false;
		// Leben die Gegner noch?
		gesamtstaerke = 0;
		for(int i=0; i < anzahl; i++) {
			if(gegner[i].staerke < 0)
				gegner[i].staerke = 0;
			gesamtstaerke += gegner[i].staerke;
		}
	}
	if(spieler->staerke > 0)
		return true; // Sieger im Kampf ^.^
	return false; // Der Spieler ist tot v.v
}

// Implementation: Momentane Werte
void momentane_werte(charakter_t *person) {
	printw("\n      Name: %s\n", person->name);
	printw("Gewandheit: %2d / %2d\n", person->gewandheit, person->gewandheit_start);
	printw("    Stärke: %2d / %2d\n", person->staerke, person->staerke_start);
	printw("     Glück: %2d / %2d\n", person->glueck, person->glueck_start);
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
void textausgabe(char *gesamttext) {
	int zeilenlaenge = COLS; // COLS ist eine ncurses Variable
	int maxzeilen = LINES; // LINES ist eine ncurses Variable

	char *resttext = gesamttext;
	char textzeile[zeilenlaenge];
	int i; // Schleifenzähler
	int j; // Schleifenzähler
	int zeile = 0;
	bool erstausgabe = true;
	int x, y;

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
/*  getyx(stdscr, y, x); // Cursorposition feststellen
	if((erstausgabe == true) && (y >= (maxzeilen - 2))) {
		weiter();
		zeile = 0;
		erstausgabe = false;
	}
*/
	if(zeile == (maxzeilen - 1)) {
			weiter();
			zeile = 0;
	}
	printw("%s\n", resttext);
	refresh();
}

// Implementation: Weiter
void weiter(void) {
	printw("--- Bitte ENTER-Taste drücken um fortzufahren ---");
	getch();
	clear(); // Bildschirm löschen
	curs_set(0);
}

// Implementation: Auswahl
void auswahl(char *beschreibung, int maxzahl, void (*funktion1)(), void (*funktion2)(), void (*funktion3)(), void (*funktion4)(), void (*funktion5)(), void (*funktion6)()) {
	char eingabe[20];
	int ergebnis = 0;
	char zusatzbeschreibung[300];

	while((ergebnis < 1) || (ergebnis > maxzahl)) {
		color_set(1, 0);
		strcpy(zusatzbeschreibung, " ");
		if(objekt[gewandheitstrank] > 0)
			strcat(zusatzbeschreibung, "(7) Gewandheitstrank trinken ");
		if(objekt[staerketrank] > 0)
			strcat(zusatzbeschreibung, "(8) Stärketrank trinken ");
		if(objekt[glueckstrank] > 0)
			strcat(zusatzbeschreibung, "(9) Glückstrank trinken ");
			strcat(zusatzbeschreibung, "(77) Spielstand laden (88) Spielstand speichern (99) Speichern & beenden");
		textausgabe(beschreibung);
		textausgabe(zusatzbeschreibung);
		getnstr(eingabe, 20);
		ergebnis = atoi(eingabe);
		color_set(0, 0);
		switch(ergebnis) {
			case 7: gewandheitstrank_trinken();
					break;
			case 8: staerketrank_trinken();
					break;
			case 9: glueckstrank_trinken();
					break;
			case 77: laden();
					 break;
			case 88: speichern();
					 break;
			case 99: speichern();
					 color_set(2, 0);
					 printw("Bis bald!\n");
					 color_set(0, 0);
					 refresh();
					 exit(EXIT_SUCCESS);
			default: break;
		}
	}
	switch(ergebnis) {
		case 1: funktion1();
				break;
				;;
		case 2: funktion2();
				break;
				;;
		case 3: funktion3();
				break;
				;;
		case 4: funktion4();
				break;
				;;
		case 5: funktion5();
				break;
				;;
		case 6: funktion6();
				break;
				;;
		default: fputs("FEHLER! Das Ergebnis einer Auswahlfunktion war nicht gültig.", stdin);
				 exit(EXIT_FAILURE);
	}
}

// Funktion: VersucheDeinGlueck
void versuchedeinglueck(void (*funktion1)(), void (*funktion2)()) {
	color_set(1, 0);
	textausgabe("--- Versuche dein Glück! - Drücke ENTER ---");
//	getchar();
	getch();
	color_set(0, 0);
	if(tdg(&spieler)) // Glück gehabt
		funktion1();
	else 			// Pech gehabt
		funktion2();
}

// Funktion: VersucheDeineGewandheit
void versuchedeinegewandheit(void (*funktion1)(), void (*funktion2)()) {
	bool gewandheit;

	color_set(1, 0);
	textausgabe("--- Versuche deine Gewandheit! - Drücke ENTER ---");
	getch();
	color_set(0, 0);
	gewandheit = wuerfel(6) + wuerfel(6);
	printw("Deine momentane Gewandheit ist %d, dein Würfelergebnis ist %d\n", spieler.gewandheit, gewandheit);
	if(gewandheit <= spieler.gewandheit) // Glück gehabt
		funktion1();
	else 			// Pech gehabt
		funktion2();
}

// Implementation: waehle
int waehle(char* beschreibung, int maxzahl) {
	int ergebnis;
	char eingabe[20];

	color_set(1, 0);
	while((ergebnis < 1) || (ergebnis > maxzahl)) {
		textausgabe(beschreibung);
		getnstr(eingabe, 20);
		ergebnis = atoi(eingabe);
	}
	color_set(0, 0);
	return ergebnis;
}

// Funktion: Flucht
void flucht(void (*funktion1)()) {
	textausgabe("Du entschließt dich zu fliehen, was dich 2 Stärkepunkte kosten wird.");
	if(spieler.glueck > 0) { // Ist ein Glückstest möglich?
		textausgabe("Du könntest natürlich auch dein Glück auf die Probe stellen. Hast du Glück, ist es eine geordnete Flucht - und du verlierst nur 1 Stärkepunkt. Wenn du jedoch kein Glück hast, wird es zu einer heilosen Flucht und du verlierst 3 Stärkepunkte.");
		if(janeinfrage("Möchtest du dein Glück testen (j/n)? ")) {
			if(tdg(&spieler)) // Glück gehabt
				spieler.staerke -= 1;
			else // Pech gehabt
				spieler.staerke -= 3;
		}
		else
			spieler.staerke -= 2;
	}
	else {
		spieler.staerke -= 2;
	}
	if(spieler.staerke) {
		funktion1();
		color_set(4, 0);
		printw("Fehler! Eine Ortsvariable ist wohl noch leer.");
		exit(EXIT_FAILURE);
	}
	else {
		textausgabe("Du bist zu geschwächt, um auch nur einen einzigen weiteren Atemzug zu machen. Unter Schmerzen entweicht dir Atemluft aus deinen Lungen. Die Umgebung wird erst Rot vor deinen Augen und gleitet schließlich ins Schwarze ab. Das ist dein ENDE.");
		exit(EXIT_SUCCESS);
	}
	color_set(4, 0);
	printw("Fehler! Ich bin am Ende der Fluchtroutine angelangt. Der letzte bekannte Raum war %d.", raum);
	exit(EXIT_FAILURE);
}

// Implementation: Mahlzeit
void mahlzeit(void) {
	if(janeinfrage("Möchtest du eine Mahlzeit zu dir nehmen (j/n)? "))
		if(objekt[proviant] > 0) {
			textausgabe("Nachdem du dich versichert hast, das niemand in der Nähe ist, entnimmst du ein Proviantpaket aus deinem Rucksack. Genüsslich und so leise wie möglich verzehrst du es. Du kannst spüren, wie etwas Kraft in deinen müden Körper zurückkehrt.");
			objekt[proviant] -= 1;
			staerkesteigerung(4, 0);
		}
		else
			textausgabe("Nachdem du dich versichert hast, das niemand in der Nähe ist, durchwühlst du deinen Rucksack auf der Suche nach einem Proviantpaket. Nach einigen Minuten und mehrfachen aus- und einpacken des Rucksacks gibst du verzweifelt auf. Es ist tatsächlich kein einziger Brotkrummen mehr übrig.");
}

// Funktion: Gewandheitstrank trinken
void gewandheitstrank_trinken() {
	if(objekt[gewandheitstrank] > 0) {
		objekt[gewandheitstrank] -= 1;
		spieler.gewandheit = spieler.gewandheit_start;
		textausgabe("Du trinkst den Gewandheitstrank. Deine Gewandheit könnte nicht besser sein.");
	}
}

// Funktion: Stärketrank trinken
void staerketrank_trinken() {
	if(objekt[staerketrank]) {
		objekt[staerketrank] -= 1;
		spieler.staerke = spieler.staerke_start;
		textausgabe("Du trinkst den Stärketrank. Deine Stärke könnte nicht besser sein.");
	}
}

// Funktion Glückstrank trinken
void glueckstrank_trinken() {
	if(objekt[glueckstrank]) {
		objekt[glueckstrank] -= 1;
		spieler.glueck_start += 1;
		spieler.glueck = spieler.glueck_start;
		textausgabe("Du trinkst den Glückstrank. Deine Glück ist besser als jemals zuvor.");
	}
}

// Implementation: Objkekt ablegen
void objekt_ablegen(void) {
	char eingabe[21];
	int ergebnis = -1;
	char bestaetigung;
	int j=0;

	for(int i=0; (i < maximalobjekt) && (objekt[i] > 0); i++) {
		printw("(%d) %s ", objekt[i], objektname[i]);
		j++;
		if(!(j % 3))
			printw("\n");
	}
	while((ergebnis < 0) || (ergebnis >= maximalobjekt)) {
		textausgabe("Bitte gib die Nummer des abzulegenden Objektes an! ");
		getnstr(eingabe, 20);
		ergebnis = atoi(eingabe);
	}
	color_set(1, 0);
	printw("%s wirklich ablegen? ", objektname[ergebnis]);
	bestaetigung = taste();
	color_set(0, 0);
	if((bestaetigung == 'j') || (bestaetigung == 'J')) {
	objekt[ergebnis] -= 1;
		printw("\n%s abgelegt.\n", objektname[ergebnis]);
	}
}

// Implementation: Ja-Nein-Frage
bool janeinfrage(char *frage) {
	char eingabe;
	color_set(1, 0);
	textausgabe(frage);
	eingabe = taste();
	color_set(0, 0);
	if((eingabe == 'j') || (eingabe == 'J'))
		return true;
	else
		return false;
}

// Implementation: gewandheitssteigerung
void gewandheitssteigerung(int temporaer, int permanent) {
	spieler.gewandheit_start += permanent;
	spieler.gewandheit += temporaer;
	if(spieler.gewandheit > spieler.gewandheit_start)
		spieler.gewandheit = spieler.gewandheit_start;
}

// Implementation: staerkesteigerung
void staerkesteigerung(int temporaer, int permanent) {
	spieler.staerke_start += permanent;
	spieler.staerke+= temporaer;
	if(spieler.staerke > spieler.staerke_start)
		spieler.staerke = spieler.staerke_start;
}

// Implementation: glueckssteigerung
void glueckssteigerung(int temporaer, int permanent) {
	spieler.glueck_start += permanent;
	spieler.glueck += temporaer;
	if(spieler.glueck > spieler.glueck_start)
		spieler.glueck = spieler.glueck_start;
}

// -------------------------
// Implementation: Speichern
// -------------------------

int speichern(void) {
	FILE *datei;
	datei = fopen(DATEINAME, "w");
	if(ferror(datei)) {
		color_set(4, 0);
		printw("Die Datei läßt sich incht speichern. Fahre ohne gespeicherten Spielstand fort.");
		color_set(0, 0);
		return 1;
	}

	fprintf(datei, "%s\n", spieler.name);
	fprintf(datei, "%d\n", spieler.gewandheit);
	fprintf(datei, "%d\n", spieler.gewandheit_start);
	fprintf(datei, "%d\n", spieler.glueck);
	fprintf(datei, "%d\n", spieler.glueck_start);
	fprintf(datei, "%d\n", spieler.staerke);
	fprintf(datei, "%d\n", spieler.staerke_start);
	fprintf(datei, "%d\n", raum);
	for(int i=0; i<maximalobjekt; i++)
		fprintf(datei, "%d\n", objekt[i]);
	fprintf(datei, "%d\n", angriffsbonus);
	fprintf(datei, "%d\n", fuenfwahl);
	fprintf(datei, "%d\n", (int) nursilbertrifft);
	fprintf(datei, "%d\n", paralysiert);
	fprintf(datei, "%d\n", preis);
	fprintf(datei, "%d\n", schluessel);
	fprintf(datei, "%d\n", (int) schluessel9);
	fprintf(datei, "%d\n", (int) schluessel66);
	fprintf(datei, "%d\n", (int) schluessel99);
	fprintf(datei, "%d\n", (int) schluessel111_1);
	fprintf(datei, "%d\n", (int) schluessel111_2);
	fprintf(datei, "%d\n", (int) schluessel125);
	fprintf(datei, "%d\n", (int) schluesselbootshaus);
	fprintf(datei, "%d\n", (int) schummeln);
	fprintf(datei, "%d\n", (int) silberwaffe);
	fprintf(datei, "%d\n", (int) unsichtbar);
	fprintf(datei, "%d\n", (int) tripodgesehen);
	fprintf(datei, "%d\n", getoetetemenschen);
	fprintf(datei, "%d\n", getoetetegegner);
	fprintf(datei, "%d\n", (int) agartha);
	fprintf(datei, "%d\n", (int) verzeichnisgelesen);
	fprintf(datei, "%d\n", (int) buchgefunden);
	fprintf(datei, "%d\n", (int) kartegefunden);
	fprintf(datei, "%d\n", (int) sargverschoben);
	fprintf(datei, "%d\n", (int) durchganggeoeffnet);
	fprintf(datei, "%d\n", (int) schluesselgefunden);
	fclose(datei);

	color_set(3, 0);
	printw("Spielstand gespeichert.\n");
	printw("Raum: %d\n", raum);
	color_set(0, 0);
	return 0;
}


// -------------------------
// Implementation: Laden
// -------------------------

int laden(void) {
	char eingabe[100];
	FILE *datei;
	datei = fopen(DATEINAME, "r");
	if(ferror(datei)) {
		color_set(4, 0);
		printw("Die Datei ließ sich nicht öffnen. Fahre ohne geladenen Spielstand fort.");
		color_set(0, 0);
		return 1;
	}
	fgets(eingabe, 100, datei);
	for(int i=0; i < strlen(eingabe); i++)
		if(eingabe[i] == '\n')
			eingabe[i] = '\0';
	strcpy(spieler.name, eingabe);
	fgets(eingabe, 100, datei);
	spieler.gewandheit = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.gewandheit_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.glueck = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.glueck_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.staerke = atoi(eingabe);
	fgets(eingabe, 100, datei);
	spieler.staerke_start = atoi(eingabe);
	fgets(eingabe, 100, datei);
	raum = atoi(eingabe);
	for(int i=0; i<maximalobjekt; i++) {
		fgets(eingabe, 100, datei);
		objekt[i] = atoi(eingabe);
	}
	fgets(eingabe, 100, datei);
	angriffsbonus = atoi(eingabe);
	fgets(eingabe, 100, datei);
	fuenfwahl = atoi(eingabe);
	fgets(eingabe, 100, datei);
	nursilbertrifft = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	paralysiert = atoi(eingabe);
	fgets(eingabe, 100, datei);
	preis = atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel = atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel9 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel66 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel99 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel111_1 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel111_2 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluessel125 = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluesselbootshaus = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schummeln = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	silberwaffe = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	unsichtbar = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	tripodgesehen = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	getoetetemenschen = atoi(eingabe);
	fgets(eingabe, 100, datei);
	getoetetegegner = atoi(eingabe);
	fgets(eingabe, 100, datei);
	agartha = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	verzeichnisgelesen = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	buchgefunden = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	kartegefunden = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	sargverschoben = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	durchganggeoeffnet = (bool) atoi(eingabe);
	fgets(eingabe, 100, datei);
	schluesselgefunden = (bool) atoi(eingabe);
	fclose(datei);

	color_set(3, 0);
	printw("Spielstand geladen.\n");
	momentane_werte(&spieler);
	printw("Raum: %d\n", raum);
	color_set(0, 0);
	raumptr[raum](); // weiter geht's im Spiel in Raum [raum]
	return 0;
}

// Funktion: quit
void quit(void) {
	endwin();
}

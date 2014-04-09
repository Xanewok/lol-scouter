#ifndef CONSTANTS_H
#define CONSTANTS_H

static const int NUM_TEAMS = 16;
static const int NUM_TEAM_MEMBERS = 5;

static const int TEAMS[NUM_TEAMS][NUM_TEAM_MEMBERS] = {{19381878, 19739336, 26154202, 29945492, 24423271},	/* Random Five */
												{28139728, 19748815, 29943215, 31171162, 19315755},	/* Purple Siege Minions */
												{27458826, 24207536, 25163092, 26949381, 27101500},	/* Nawiedzeni */
												{34643485, 24753771, 29997052, 27983360, 25053140},	/* Kuba znowu w tyle */
												{21176576, 25227171, 35438455, 25904226, 34200780},	/* Kurczak Tajland */
												{20250492, 19370276, 19031155, 20153342, 19273528},	/* Slodkie Ziemniaczki Prime */
												{19446487, 19406317, 19708474, 19383763, 19393463},	/* Nasos pls */
												{30976455, 33093599, 20339886, 24793079, 24450367},	/* Bronzy na bocie */
												{22311549, 39633121, 22960348, 23760345, 23042154},	/* Prawilna Piatka */
												{19754347, 20976729, 346822, 33769371, 20557017},	/* Liniowo Niezalezni */
												{23940204, 39053694, 24330119, 25444452, 31181645},	/* Kenson Squad */
												{24368336, 21191718, 19878565, 22662299, 19863192},	/* Pan da RP-ki */
												{20095198, 20072326, 19928525, 19940196, 19811451},	/* AGT */
												{28190588, 23918752, 23982127, 27797169, 24094826}, /* Kule po dziurach */
												{33863101, 28545674, 33863166, 34333251, 30972927},	/* Team Erasers */
												{20539748, 20493466, 21803047, 26121772, 27824463}};/* Baited */

static const char* SUMMONER_NAMES[NUM_TEAMS][NUM_TEAM_MEMBERS] = {{"Serphen", "Zecik", "sn4p1k", "eXcruZer", "MelamanDre"},	/* Random Five */
														{"Vherax", "DaxiForever", "NoobzPwnr", "ListerTormentor", "xbiti"},	/* Purple Siege Minions */
														{"fr3dziu", "NoName139", "malyfrankus", "5booze", "SchmetterlingPL"},	/* Nawiedzeni */
														{"eugheeniush", "Inazuma Yuki", "Kat Inter123", "hash3r", "kometkemot"},	/* Kuba znowu w tyle */
														{"Wreat", "GrubyKingston", "zZzblazZz", "Christofano", "Sebnar"},	/* Kurczak Tajland */
														{"ALSEN Mataha", "Niezapominalski", "IMP Szychu", "aSpera MrMad", "LosoS"},	/* Slodkie Ziemniaczki Prime */
														{"Guguar", "Galbatarix", "S0urce", "Mam Trzy Pachy", "Pogromca Balsamu"},	/* Nasos pls */
														{"Legwan69", "Krzycz Mjuton", "Wbps Jager", "Malutki69", "Mekee7"},	/* Bronzy na bocie */
														{"Panterro", "Sunmile", "Dynamitwypas", "Nowilus", "KrolSucharus"},	/* Prawilna Piatka */
														{"theValdis", "KaKaDu", "KBWariatka", "Thaeldtai", "FnytMaster"},	/* Liniowo Niezalezni */
														{"Tidusek", "Kenson", "Qbaz", "Kayen", "kondziorex90"},	/* Kenson Squad */
														{"Idzin", "Woql", "Wazonia", "Meferisto", "Ithnael"},	/* Pan da RP-ki */
														{"Natella", "Roff", "Drynda", "adamm", "EniGGmatic"},	/* AGT */
														{"Jakubeusz", "Pan Psor", "Szon Bejn", "HejMakiMaki", "MaroB"}, /* Kule po dziurach */
														{"HasanSalim", "paNiczdg", "Equivet", "ed0sno", "That D4ro"},	/* Team Erasers */
														{"HadorX", "Tandraks", "Ksalk", "Meanderr", "Lifeless Reaver"}};/* Baited */
static const char * SUMMONER_SPELLS[22] = {0, "Cleanse", "Clairvoyance", "Exhaust", "Flash", 0, "Ghost", "Heal", 0, 0, "Revive", "Smite", "Teleport", "Clarity", "Ignite", 0, 0, "Garrison", 0, 0, 0, "Barrier"};
#endif
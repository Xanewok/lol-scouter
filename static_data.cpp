#include "static_data.h"


const int TEAMS[NUM_TEAMS][NUM_TEAM_MEMBERS] = {{19381878, 19739336, 26154202, 29945492, 24423271},	/* Random Five */
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
/* Maximum length of a summoner name is 16 characters long. */
char const * SUMMONER_NAMES[NUM_TEAMS][NUM_TEAM_MEMBERS] = {{"Serphen", "Zecik", "sn4p1k", "eXcruZer", "MelamanDre"},	/* Random Five */
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

char const * SUMMONER_SPELLS[22] = {0, "Cleanse", "Clairvoyance", "Exhaust", "Flash", 0, "Ghost", "Heal", 0, 0, "Revive", "Smite", "Teleport", "Clarity", "Ignite", 0, 0, "Garrison", 0, 0, 0, "Barrier"};


int find_team(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++)
			if (TEAMS[i][j] == summoner_id)
				return i;
	return -1;
}

int find_summoner(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++)
			if (TEAMS[i][j] == summoner_id)
				return j;
	return -1;
}

char * get_champion_name(int champion_id)
{
	switch (champion_id) {
	case 412:
		return (char *)"Thresh";
		break;
	case 266:
		return (char *)"Aatrox";
		break;
	case 23:
		return (char *)"Tryndamere";
		break;
	case 79:
		return (char *)"Gragas";
		break;
	case 69:
		return (char *)"Cassiopeia";
		break;
	case 13:
		return (char *)"Ryze";
		break;
	case 78:
		return (char *)"Poppy";
		break;
	case 14:
		return (char *)"Sion";
		break;
	case 1:
		return (char *)"Annie";
		break;
	case 111:
		return (char *)"Nautilus";
		break;
	case 43:
		return (char *)"Karma";
		break;
	case 99:
		return (char *)"Lux";
		break;
	case 103:
		return (char *)"Ahri";
		break;
	case 2:
		return (char *)"Olaf";
		break;
	case 112:
		return (char *)"Viktor";
		break;
	case 34:
		return (char *)"Anivia";
		break;
	case 86:
		return (char *)"Garen";
		break;
	case 27:
		return (char *)"Singed";
		break;
	case 127:
		return (char *)"Lissandra";
		break;
	case 57:
		return (char *)"Maokai";
		break;
	case 25:
		return (char *)"Morgana";
		break;
	case 28:
		return (char *)"Evelynn";
		break;
	case 105:
		return (char *)"Fizz";
		break;
	case 238:
		return (char *)"Zed";
		break;
	case 74:
		return (char *)"Heimerdinger";
		break;
	case 68:
		return (char *)"Rumble";
		break;
	case 37:
		return (char *)"Sona";
		break;
	case 82:
		return (char *)"Mordekaiser";
		break;
	case 96:
		return (char *)"Kog'Maw";
		break;
	case 55:
		return (char *)"Katarina";
		break;
	case 117:
		return (char *)"Lulu";
		break;
	case 22:
		return (char *)"Ashe";
		break;
	case 30:
		return (char *)"Karthus";
		break;
	case 12:
		return (char *)"Alistar";
		break;
	case 122:
		return (char *)"Darius";
		break;
	case 67:
		return (char *)"Vayne";
		break;
	case 77:
		return (char *)"Udyr";
		break;
	case 110:
		return (char *)"Varus";
		break;
	case 89:
		return (char *)"Leona";
		break;
	case 126:
		return (char *)"Jayce";
		break;
	case 134:
		return (char *)"Syndra";
		break;
	case 80:
		return (char *)"Pantheon";
		break;
	case 92:
		return (char *)"Riven";
		break;
	case 121:
		return (char *)"Kha'Zix";
		break;
	case 42:
		return (char *)"Corki";
		break;
	case 51:
		return (char *)"Caitlyn";
		break;
	case 76:
		return (char *)"Nidalee";
		break;
	case 85:
		return (char *)"Kennen";
		break;
	case 3:
		return (char *)"Galio";
		break;
	case 45:
		return (char *)"Veigar";
		break;
	case 104:
		return (char *)"Graves";
		break;
	case 90:
		return (char *)"Malzahar";
		break;
	case 254:
		return (char *)"Vi";
		break;
	case 10:
		return (char *)"Kayle";
		break;
	case 39:
		return (char *)"Irelia";
		break;
	case 64:
		return (char *)"Lee Sin";
		break;
	case 60:
		return (char *)"Elise";
		break;
	case 106:
		return (char *)"Volibear";
		break;
	case 20:
		return (char *)"Nunu";
		break;
	case 4:
		return (char *)"Twisted Fate";
		break;
	case 24:
		return (char *)"Jax";
		break;
	case 102:
		return (char *)"Shyvana";
		break;
	case 36:
		return (char *)"Dr. Mundo";
		break;
	case 63:
		return (char *)"Brand";
		break;
	case 131:
		return (char *)"Diana";
		break;
	case 113:
		return (char *)"Sejuani";
		break;
	case 8:
		return (char *)"Vladimir";
		break;
	case 154:
		return (char *)"Zac";
		break;
	case 133:
		return (char *)"Quinn";
		break;
	case 84:
		return (char *)"Akali";
		break;
	case 18:
		return (char *)"Tristana";
		break;
	case 120:
		return (char *)"Hecarim";
		break;
	case 15:
		return (char *)"Sivir";
		break;
	case 236:
		return (char *)"Lucian";
		break;
	case 107:
		return (char *)"Rengar";
		break;
	case 19:
		return (char *)"Warwick";
		break;
	case 72:
		return (char *)"Skarner";
		break;
	case 54:
		return (char *)"Malphite";
		break;
	case 157:
		return (char *)"Yasuo";
		break;
	case 101:
		return (char *)"Xerath";
		break;
	case 17:
		return (char *)"Teemo";
		break;
	case 75:
		return (char *)"Nasus";
		break;
	case 58:
		return (char *)"Renekton";
		break;
	case 119:
		return (char *)"Draven";
		break;
	case 35:
		return (char *)"Shaco";
		break;
	case 50:
		return (char *)"Swain";
		break;
	case 115:
		return (char *)"Ziggs";
		break;
	case 40:
		return (char *)"Janna";
		break;
	case 91:
		return (char *)"Talon";
		break;
	case 61:
		return (char *)"Orianna";
		break;
	case 9:
		return (char *)"Fiddlesticks";
		break;
	case 114:
		return (char *)"Fiora";
		break;
	case 31:
		return (char *)"Cho'Gath";
		break;
	case 33:
		return (char *)"Rammus";
		break;
	case 7:
		return (char *)"LeBlanc";
		break;
	case 26:
		return (char *)"Zilean";
		break;
	case 16:
		return (char *)"Soraka";
		break;
	case 56:
		return (char *)"Nocturne";
		break;
	case 222:
		return (char *)"Jinx";
		break;
	case 83:
		return (char *)"Yorick";
		break;
	case 6:
		return (char *)"Urgot";
		break;
	case 21:
		return (char *)"Miss Fortune";
		break;
	case 62:
		return (char *)"Wukong";
		break;
	case 53:
		return (char *)"Blitzcrank";
		break;
	case 98:
		return (char *)"Shen";
		break;
	case 5:
		return (char *)"Xin Zhao";
		break;
	case 29:
		return (char *)"Twitch";
		break;
	case 11:
		return (char *)"Master Yi";
		break;
	case 44:
		return (char *)"Taric";
		break;
	case 32:
		return (char *)"Amumu";
		break;
	case 41:
		return (char *)"Gangplank";
		break;
	case 48:
		return (char *)"Trundle";
		break;
	case 38:
		return (char *)"Kassadin";
		break;
	case 161:
		return (char *)"Vel'Koz";
		break;
	case 143:
		return (char *)"Zyra";
		break;
	case 267:
		return (char *)"Nami";
		break;
	case 59:
		return (char *)"Jarvan IV";
		break;
	case 81:
		return (char *)"Ezreal";
		break;
	default:
		return (char *)"No Champion Found";
		break;
	}
}
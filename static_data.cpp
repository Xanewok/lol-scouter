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
		return "Thresh";
		break;
	case 266:
		return "Aatrox";
		break;
	case 23:
		return "Tryndamere";
		break;
	case 79:
		return "Gragas";
		break;
	case 69:
		return "Cassiopeia";
		break;
	case 13:
		return "Ryze";
		break;
	case 78:
		return "Poppy";
		break;
	case 14:
		return "Sion";
		break;
	case 1:
		return "Annie";
		break;
	case 111:
		return "Nautilus";
		break;
	case 43:
		return "Karma";
		break;
	case 99:
		return "Lux";
		break;
	case 103:
		return "Ahri";
		break;
	case 2:
		return "Olaf";
		break;
	case 112:
		return "Viktor";
		break;
	case 34:
		return "Anivia";
		break;
	case 86:
		return "Garen";
		break;
	case 27:
		return "Singed";
		break;
	case 127:
		return "Lissandra";
		break;
	case 57:
		return "Maokai";
		break;
	case 25:
		return "Morgana";
		break;
	case 28:
		return "Evelynn";
		break;
	case 105:
		return "Fizz";
		break;
	case 238:
		return "Zed";
		break;
	case 74:
		return "Heimerdinger";
		break;
	case 68:
		return "Rumble";
		break;
	case 37:
		return "Sona";
		break;
	case 82:
		return "Mordekaiser";
		break;
	case 96:
		return "Kog'Maw";
		break;
	case 55:
		return "Katarina";
		break;
	case 117:
		return "Lulu";
		break;
	case 22:
		return "Ashe";
		break;
	case 30:
		return "Karthus";
		break;
	case 12:
		return "Alistar";
		break;
	case 122:
		return "Darius";
		break;
	case 67:
		return "Vayne";
		break;
	case 77:
		return "Udyr";
		break;
	case 110:
		return "Varus";
		break;
	case 89:
		return "Leona";
		break;
	case 126:
		return "Jayce";
		break;
	case 134:
		return "Syndra";
		break;
	case 80:
		return "Pantheon";
		break;
	case 92:
		return "Riven";
		break;
	case 121:
		return "Kha'Zix";
		break;
	case 42:
		return "Corki";
		break;
	case 51:
		return "Caitlyn";
		break;
	case 76:
		return "Nidalee";
		break;
	case 85:
		return "Kennen";
		break;
	case 3:
		return "Galio";
		break;
	case 45:
		return "Veigar";
		break;
	case 104:
		return "Graves";
		break;
	case 90:
		return "Malzahar";
		break;
	case 254:
		return "Vi";
		break;
	case 10:
		return "Kayle";
		break;
	case 39:
		return "Irelia";
		break;
	case 64:
		return "Lee Sin";
		break;
	case 60:
		return "Elise";
		break;
	case 106:
		return "Volibear";
		break;
	case 20:
		return "Nunu";
		break;
	case 4:
		return "Twisted Fate";
		break;
	case 24:
		return "Jax";
		break;
	case 102:
		return "Shyvana";
		break;
	case 36:
		return "Dr. Mundo";
		break;
	case 63:
		return "Brand";
		break;
	case 131:
		return "Diana";
		break;
	case 113:
		return "Sejuani";
		break;
	case 8:
		return "Vladimir";
		break;
	case 154:
		return "Zac";
		break;
	case 133:
		return "Quinn";
		break;
	case 84:
		return "Akali";
		break;
	case 18:
		return "Tristana";
		break;
	case 120:
		return "Hecarim";
		break;
	case 15:
		return "Sivir";
		break;
	case 236:
		return "Lucian";
		break;
	case 107:
		return "Rengar";
		break;
	case 19:
		return "Warwick";
		break;
	case 72:
		return "Skarner";
		break;
	case 54:
		return "Malphite";
		break;
	case 157:
		return "Yasuo";
		break;
	case 101:
		return "Xerath";
		break;
	case 17:
		return "Teemo";
		break;
	case 75:
		return "Nasus";
		break;
	case 58:
		return "Renekton";
		break;
	case 119:
		return "Draven";
		break;
	case 35:
		return "Shaco";
		break;
	case 50:
		return "Swain";
		break;
	case 115:
		return "Ziggs";
		break;
	case 40:
		return "Janna";
		break;
	case 91:
		return "Talon";
		break;
	case 61:
		return "Orianna";
		break;
	case 9:
		return "Fiddlesticks";
		break;
	case 114:
		return "Fiora";
		break;
	case 31:
		return "Cho'Gath";
		break;
	case 33:
		return "Rammus";
		break;
	case 7:
		return "LeBlanc";
		break;
	case 26:
		return "Zilean";
		break;
	case 16:
		return "Soraka";
		break;
	case 56:
		return "Nocturne";
		break;
	case 222:
		return "Jinx";
		break;
	case 83:
		return "Yorick";
		break;
	case 6:
		return "Urgot";
		break;
	case 21:
		return "Miss Fortune";
		break;
	case 62:
		return "Wukong";
		break;
	case 53:
		return "Blitzcrank";
		break;
	case 98:
		return "Shen";
		break;
	case 5:
		return "Xin Zhao";
		break;
	case 29:
		return "Twitch";
		break;
	case 11:
		return "Master Yi";
		break;
	case 44:
		return "Taric";
		break;
	case 32:
		return "Amumu";
		break;
	case 41:
		return "Gangplank";
		break;
	case 48:
		return "Trundle";
		break;
	case 38:
		return "Kassadin";
		break;
	case 161:
		return "Vel'Koz";
		break;
	case 143:
		return "Zyra";
		break;
	case 267:
		return "Nami";
		break;
	case 59:
		return "Jarvan IV";
		break;
	case 81:
		return "Ezreal";
		break;
	default:
		return "No Champion Found";
		break;
	}
}
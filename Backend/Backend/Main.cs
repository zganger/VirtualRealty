using System;
using System.Collections;

namespace Backend
{
	class MainClass
	{
		public static void Main(string[] args)
		{
			Console.WriteLine ("I'm an idiot");
			Board GameBoard = new Board ();
		}
	}
	class Board
	{
		ArrayList tiles = new ArrayList();
		ArrayList pieces = new ArrayList();

		public Board()
		{
			pieces.Add (new Piece ("Racecar", "Red", 0));
			pieces.Add (new Piece ("Tophat", "Black", 1));
			int[] values = new int[40] {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400};
			string[] spaces = new string[40]{"Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"};
			int[] mortgages = new int[40] {0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200};
			string[] colorGroups = new string[40]{null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"};
			int[,] rents = new int[40, 6] { {-200, -200, -200, -200, -200, -200}, {2, 10, 30, 90, 160, 250}, {0, 0, 0, 0, 0, 0}, {4, 20, 60, 180, 320, 450}, {200, 200, 200, 200, 200, 200}, {25, 50, 100, 200, 200, 200}, {6, 30, 90, 270, 400, 550}, {0, 0, 0, 0, 0, 0}, {6, 30, 90, 270, 400, 550}, {8, 40, 100, 300, 450, 600}, {0, 0, 0, 0, 0, 0}, {10, 50, 150, 450, 625, 750}, {0, 0, 0, 0, 0, 0}, {10, 50, 150, 450, 625, 750}, {12, 60, 180, 500, 700, 900}, {25, 50, 100, 200, 200, 200}, {14, 70, 200, 550, 750, 950}, {0, 0, 0, 0, 0, 0}, {14, 70, 200, 550, 750, 950}, {16, 80, 220, 600, 800, 1000}, {0, 0, 0, 0, 0, 0}, {18, 90, 250, 700, 875, 1050}, {0, 0, 0, 0, 0, 0}, {18, 90, 250, 700, 875, 1050}, {20, 100, 300, 750, 925, 1100}, {25, 50, 100, 200, 200, 200}, {22, 110, 330, 800, 975, 1150}, {22, 110, 330, 800, 975, 1150}, {0, 0, 0, 0, 0, 0}, {24, 120, 360, 850, 1025, 1200}, {0, 0, 0, 0, 0, 0}, {26, 130, 390, 900, 1100, 1275}, {26, 130, 390, 900, 1100, 1275}, {0, 0, 0, 0, 0, 0}, {28, 150, 450, 1000, 1200, 1400}, {25, 50, 100, 200, 200, 200}, {0, 0, 0, 0, 0, 0}, {35, 175, 500, 1100, 1300, 1500}, {100, 100, 100, 100, 100, 100}, {50, 200, 600, 1400, 1700, 2000} };
			int[] passRents = new int[6];
			for (int i = 0; i < 40; i++) {
				for(int jj = 0; jj < 6; jj++)
				{
					passRents[jj] = rents[i,jj];
				}
				tiles.Add(new Tile(spaces[i], values[i], passRents, mortgages[i], colorGroups[i]));
			}
			Console.WriteLine("BOARD COMPLETE");
		}
	}

	class Piece
	{
		Owner player;
		string type;
		int doubcount;
		bool isJailed;
		int location;
		public Piece(string in_type, string color, int id)
		{
			player = new Owner (color, id);
			type = in_type;
			doubcount = 0;
			isJailed = false;
			location = 0;
			Console.WriteLine (type + " created");
		}
	}

	class Tile
	{
		string title;
		Property property;
		bool isProperty;
		public Tile(string in_title, int value, int[] rents, int mortgage, string colorGroup)
		{
			title = in_title;
			Console.WriteLine (title + " created");
			if (value == 0) {
				isProperty = false;
			} else {
				isProperty = true;
				property = new Property(title, value, rents, mortgage, colorGroup);
			}
		}
	}

	class Owner
	{
		int ID;
		ArrayList properties = new ArrayList();
		int money;
		bool hasFreeEscape;
		string color;

		public Owner(string in_color, int in_id)
		{
			ID = in_id;
			money = 1500;
			hasFreeEscape = false;
			color = in_color;
			Console.WriteLine("Player " + ID + " created");
		}
	}

	class Property
	{
		int mortgage;
		bool isMortgaged;
		int value;
		int buildings;
		int[] rents = new int[6];
		string colorGroup;
		Owner player;

		public Property(string in_title, int in_value, int[] in_rents, int in_mortgage, string in_colorGroup)
		{
			mortgage = in_mortgage;
			value = in_value;
			isMortgaged = false;
			buildings = 0;
			rents = in_rents;
			colorGroup = in_colorGroup;
			player = null; //not yet owned
			Console.WriteLine ("property created");
		}

		public void setOwned(Owner cplayer)
		{
			player = cplayer;
		}

		public void addBuilding()
		{
			buildings++;
		}
	}

	class Card
	{
		bool type; //comm chest or chance
		string text;
		Action command;

		public Card(bool in_type, string in_text, bool m, int value)
		{
			type = in_type;
			text = in_text;
			command = new Action (m, value);
		}
	}

	class Action
	{
		bool m; //money or motion
		int value;

		public Action(bool in_m, int in_value)
		{
			m = in_m;
			value = in_value;
		}
	}
}

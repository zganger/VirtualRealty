using System;
using System.Collections;
using System.Threading;

namespace Backend
{
	class MainClass
	{
		public static void Main(string[] args)
		{
			Console.WriteLine ("Game Start");
			Board GameBoard = new Board ();
			GameBoard.beginGameplay ();
		}
	}
	class Board
	{
		public ArrayList tiles = new ArrayList();
		public ArrayList pieces = new ArrayList();
		public bool gameOver = false;
		public int diceTotal;
		public Board()
		{
			pieces.Add (new Piece ("Racecar", "Red", 0));
			pieces.Add (new Piece ("Tophat", "Black", 1));
			int[] values = new int[40] {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400};
			string[] spaces = new string[40]{"Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"};
			int[] mortgages = new int[40] {0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200};
			string[] colorGroups = new string[40]{null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"};
			int[,] rents = new int[40, 6] { {-200, -200, -200, -200, -200, -200}, {2, 10, 30, 90, 160, 250}, {0, 0, 0, 0, 0, 0}, {4, 20, 60, 180, 320, 450}, {200, 200, 200, 200, 200, 200}, {25, 50, 100, 200, 200, 200}, {6, 30, 90, 270, 400, 550}, {0, 0, 0, 0, 0, 0}, {6, 30, 90, 270, 400, 550}, {8, 40, 100, 300, 450, 600}, {0, 0, 0, 0, 0, 0}, {10, 50, 150, 450, 625, 750}, {0, 0, 0, 0, 0, 0}, {10, 50, 150, 450, 625, 750}, {12, 60, 180, 500, 700, 900}, {25, 50, 100, 200, 200, 200}, {14, 70, 200, 550, 750, 950}, {0, 0, 0, 0, 0, 0}, {14, 70, 200, 550, 750, 950}, {16, 80, 220, 600, 800, 1000}, {0, 0, 0, 0, 0, 0}, {18, 90, 250, 700, 875, 1050}, {0, 0, 0, 0, 0, 0}, {18, 90, 250, 700, 875, 1050}, {20, 100, 300, 750, 925, 1100}, {25, 50, 100, 200, 200, 200}, {22, 110, 330, 800, 975, 1150}, {22, 110, 330, 800, 975, 1150}, {0, 0, 0, 0, 0, 0}, {24, 120, 360, 850, 1025, 1200}, {0, 0, 0, 0, 0, 0}, {26, 130, 390, 900, 1100, 1275}, {26, 130, 390, 900, 1100, 1275}, {0, 0, 0, 0, 0, 0}, {28, 150, 450, 1000, 1200, 1400}, {25, 50, 100, 200, 200, 200}, {0, 0, 0, 0, 0, 0}, {35, 175, 500, 1100, 1300, 1500}, {100, 100, 100, 100, 100, 100}, {50, 200, 600, 1400, 1700, 2000} };
			for (int i = 0; i < 40; i++) {
				int[] passRents = new int[6];
				for(int jj = 0; jj < 6; jj++)
				{
					passRents[jj] = rents[i,jj];
					Console.WriteLine ("PassRents[" + jj + "] for " + spaces[i] + " is " + passRents[jj]);
				}
				tiles.Add(new Tile(spaces[i], values[i], passRents, mortgages[i], colorGroups[i]));
			}
			Console.WriteLine("BOARD COMPLETE");
		}
		public void Dice(Piece currPlayer)	//probably an int later when taking care of front end
		{
			Random rnd = new Random ((int)DateTime.Now.Ticks);
			int d1 = rnd.Next (1, 7);
			int d2 = rnd.Next (1, 7);
			this.diceTotal = d1 + d2;
			if(d1 == d2)
			{
				currPlayer.doubcount = currPlayer.doubcount + 1;
				if(currPlayer.doubcount == 3)
				{
					Console.WriteLine("Player " + currPlayer.player.ID + " has three sets of doubles and goes to jail");
					currPlayer.location = 10;
					currPlayer.isJailed = true;
					currPlayer.doubcount = 0;
					currPlayer.player.money = currPlayer.player.money - 50;
					Console.WriteLine ("Player " + currPlayer.player.ID + " pays $50 in bail"); //these two lines will be handled in jailedDice function when dooubles implemented
				}
				else
				{
					if(currPlayer.location + diceTotal > 39)
					{
						currPlayer.player.money = currPlayer.player.money + 200;
						Console.WriteLine("Player " + currPlayer.player.ID + " has passed go and collected $200. They now have $" + currPlayer.player.money);
					}
					currPlayer.location = (currPlayer.location + diceTotal) % 40;
					Console.WriteLine("Player " + currPlayer.player.ID + " Moves forward " + diceTotal + " to " + ((Tile)this.tiles[currPlayer.location]).title);
					Console.WriteLine("Doubles, go again!");
				}
			}
			else
			{
				if(currPlayer.location + diceTotal > 39)
				{
					currPlayer.player.money = currPlayer.player.money + 200;
					Console.WriteLine("Player " + currPlayer.player.ID + " has passed go and collected $200. They now have $" + currPlayer.player.money);
				}
				currPlayer.location = (currPlayer.location + diceTotal) % 40;
				Console.WriteLine("Player " + currPlayer.player.ID + " Moves forward " + diceTotal + " to " + ((Tile)this.tiles[currPlayer.location]).title);
				//console.log("You are on " + board.spaces[currPlayer.location] + ". Pay $" + board.rent[currPlayer.location][board.tiles[currPlayer.location].);  //add houses rates
				currPlayer.doubcount = 0;
			}
		}
		public void purchase(Owner landed, Property unownedSpace)
		{
			int origMoney = landed.money;
			if (landed.money >= unownedSpace.value) {
				landed.money = landed.money - unownedSpace.value;
				unownedSpace.setOwned (landed);
				landed.properties.Add (unownedSpace);
				Console.WriteLine ("Player " + landed.ID + " pays $" + unownedSpace.value + " from $" + origMoney + " to purchase " + unownedSpace.title);
				Console.WriteLine ("They now have $" + landed.money);
			} else {
				Console.WriteLine ("Not enough money to buy " + unownedSpace.title + "!");
			}
		}
		public void payRent(Owner landed, Property ownedSpace) //pay rent from piece to space owner
		{
			int rentOwed = 0;
			if (ownedSpace.title == "Electric Company") {
				bool ownsWW = false;
				foreach(Property p in ownedSpace.player.properties)
				{
					if(p.title == "Water Works")
					{
						ownsWW = true;
					}
				}
				if(ownsWW)
				{
					rentOwed = (10 * this.diceTotal);
				}
			} else if (ownedSpace.title == "Water Works") {
				bool ownsEC = false;
				foreach(Property p in ownedSpace.player.properties)
				{
					if(p.title == "Electric Company")
					{
						ownsEC = true;
					}
				}
				if(ownsEC)
				{
					rentOwed = (10 * this.diceTotal);
				}
			} else {
				rentOwed = ownedSpace.rents [ownedSpace.buildings];
			}
			landed.money = landed.money - rentOwed;
			ownedSpace.player.money = ownedSpace.player.money + rentOwed;
			Console.WriteLine("Player " + landed.ID + " pays Player " + ownedSpace.player.ID + " $" + rentOwed + " and now has $" + landed.money + ". Player " + ownedSpace.player.ID + " now has $" + ownedSpace.player.money);
			if(landed.money < 0)
			{
				Console.WriteLine("Player " + landed.ID + " loses the game");
				gameOver = true;
			}
		}
		public void beginGameplay()
		{
			//start the game
			int turncounter = 0;
			while(!gameOver)
			{
				int playerID = (turncounter % 2);
				Piece currPlayer = ((Piece)this.pieces[playerID]);
				Thread.Sleep(200);	//delay before next dice rolling
				Dice(currPlayer); //roll and move to new location
				if(((Tile)this.tiles[currPlayer.location]).isProperty) //is property
				{
					if(((Tile)this.tiles[currPlayer.location]).property.player != null) //owned
					{
						if(((Tile)this.tiles[currPlayer.location]).property.player.ID != currPlayer.player.ID) {
							payRent(currPlayer.player, ((Tile)this.tiles[currPlayer.location]).property);
						}
					}
					else //unowned
					{
						purchase(currPlayer.player, ((Tile)this.tiles[currPlayer.location]).property);
					}
				}
				else //not a property, action tile
				{
					//how to action?
					if(((Tile)this.tiles[currPlayer.location]).rents[0] != 0 && currPlayer.location != 0) //assuming action tile, only true if go or tax
					{
						currPlayer.player.money = currPlayer.player.money - ((Tile)this.tiles[currPlayer.location]).rents[0];
						Console.WriteLine("Player " + currPlayer.player.ID + " loses $" + ((Tile)this.tiles[currPlayer.location]).rents[0] + " on " + ((Tile)this.tiles[currPlayer.location]).title);
					}
					else if(currPlayer.location == 30) //go to jail
					{
						currPlayer.location = 10;
						currPlayer.isJailed = true;
					}
				}
				if(currPlayer.doubcount == 0) //end of turn, not in jail
				{
					//something for buildings and trades here
					turncounter++;
				}
				if(currPlayer.player.money < 0)//game end condition
				{
					gameOver = true;
					Console.WriteLine("Game over; Player " + currPlayer.player.ID + " has $" + currPlayer.player.money);
				}
			}
		}
	}

	class Piece
	{
		public Owner player;
		public string type;
		public int doubcount;
		public bool isJailed;
		public int location;
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
		public string title;
		public Property property;
		public bool isProperty;
		public int[] rents;
		public Tile(string in_title, int value, int[] in_rents, int mortgage, string colorGroup)
		{
			title = in_title;
			rents = in_rents;
			Console.WriteLine (title + " created");
			if (value == 0) {
				isProperty = false;
			} else {
				isProperty = true;
				property = new Property(title, value, in_rents, mortgage, colorGroup);
			}
		}
	}

	class Owner
	{
		public int ID;
		public ArrayList properties = new ArrayList();
		public int money;
		public bool hasFreeEscape;
		public string color;

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
		public int mortgage;
		public bool isMortgaged;
		public int value;
		public int buildings;
		public int[] rents = new int[6];
		public string colorGroup;
		public Owner player;
		public string title;

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
			title = in_title;
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
		public bool type; //comm chest or chance
		public string text;
		public Action command;

		public Card(bool in_type, string in_text, bool m, int value)
		{
			type = in_type;
			text = in_text;
			command = new Action (m, value);
		}
	}

	class Action
	{
		public bool m; //money or motion
		public int value;

		public Action(bool in_m, int in_value)
		{
			m = in_m;
			value = in_value;
		}
	}
}

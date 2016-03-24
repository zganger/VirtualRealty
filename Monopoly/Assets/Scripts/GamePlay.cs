using UnityEngine;
using System.Collections;
using System;
using System.Threading;
using UnityEngine.UI;


public class GamePlay : MonoBehaviour {

	public Transform target;	//where a piece is going
    public float speed;			//how fast
	public Text Balances;
	public Text Actions;
	public UnityPieceImage ShoePiece, DogPiece;
	public Image hotel1, hotel3, hotel6, hotel8, hotel9, hotel11, hotel13, hotel14, hotel16, hotel18, hotel19, hotel21, hotel23, hotel24, hotel26, hotel27, hotel29, hotel31, hotel32, hotel34, hotel37, hotel39;
	public Image house1_1, house1_2, house1_3, house1_4, house3_1, house3_2, house3_3, house3_4, house6_1, house6_2, house6_3, house6_4, house8_1, house8_2, house8_3, house8_4, house9_1, house9_2, house9_3, house9_4;
	public Image house11_1, house11_2, house11_3, house11_4, house13_1, house13_2, house13_3, house13_4, house14_1, house14_2, house14_3, house14_4, house16_1, house16_2, house16_3, house16_4, house18_1, house18_2, house18_3, house18_4, house19_1, house19_2, house19_3, house19_4;
	public Image house21_1, house21_2, house21_3, house21_4, house23_1, house23_2, house23_3, house23_4, house24_1, house24_2, house24_3, house24_4, house26_1, house26_2, house26_3, house26_4, house27_1, house27_2, house27_3, house27_4, house29_1, house29_2, house29_3, house29_4;
	public Image house31_1, house31_2, house31_3, house31_4, house32_1, house32_2, house32_3, house32_4, house34_1, house34_2, house34_3, house34_4, house37_1, house37_2, house37_3, house37_4, house39_1, house39_2, house39_3, house39_4;
	public static Board GameBoard;
	public int diceTotal;
	public bool gameOver = false;
	public ArrayList unityPieces = new ArrayList ();
	public Image[,] unityBuildings;
	public Button RentOkButton;
	public Button PropertyYes;
	public Button PropertyNo;
	public Button Roll;
	public int turncounter;

	public void Dice (Piece currPlayer, UnityPieceImage currPlayerUnity)	//probably an int later when taking care of front end
	{
		System.Random rnd = new System.Random ((int)DateTime.Now.Ticks);
		int d1 = rnd.Next (1, 7);
		int d2 = rnd.Next (1, 7);
		diceTotal = d1 + d2;
		if (d1 == d2) {
			currPlayer.doubcount = currPlayer.doubcount + 1;
			if (currPlayer.doubcount == 3) {
				Actions.text = ("Player " + currPlayer.player.ID + " has three sets of doubles and goes to jail");
				currPlayer.location = 10;
				currPlayerUnity.MoveTo (currPlayer.location);
				currPlayer.isJailed = true;
				currPlayer.doubcount = 0;
				currPlayer.player.money = currPlayer.player.money - 50; //bail immediate for the time being
				Actions.text = ("Player " + currPlayer.player.ID + " pays $50 in bail"); //these two lines will be handled in jailedDice function when doubles implemented
			} else {
				if (currPlayer.location + diceTotal > 39) {
					currPlayer.player.money = currPlayer.player.money + 200;
					Actions.text = ("Player " + currPlayer.player.ID + " has passed go and collected $200. They now have $" + currPlayer.player.money);
				}
				currPlayer.location = (currPlayer.location + diceTotal) % 40;
				currPlayerUnity.MoveTo (currPlayer.location);
				Actions.text = ("Player " + currPlayer.player.ID + " Moves forward " + diceTotal + " to " + ((Tile)GameBoard.tiles [currPlayer.location]).title);
				Actions.text = ("Doubles, go again!");
			}

		} else {
			if (currPlayer.location + diceTotal > 39) {
				currPlayer.player.money = currPlayer.player.money + 200;
				Actions.text = ("Player " + currPlayer.player.ID + " has passed go and collected $200. They now have $" + currPlayer.player.money);
			}
			currPlayer.location = (currPlayer.location + diceTotal) % 40;
			currPlayerUnity.MoveTo (currPlayer.location);
			Actions.text = ("Player " + currPlayer.player.ID + " Moves forward " + diceTotal + " to " + ((Tile)GameBoard.tiles [currPlayer.location]).title);
			currPlayer.doubcount = 0;
		}
	}

	public void purchase (Owner landed, Property unownedSpace)
	{
		int origMoney = landed.money;
		if (landed.money >= unownedSpace.value) {
			landed.money = landed.money - unownedSpace.value;
			unownedSpace.setOwned (landed);
			landed.properties.Add (unownedSpace);
			Actions.text = ("Player " + landed.ID + " pays $" + unownedSpace.value + " from $" + origMoney + " to purchase " + unownedSpace.title);
			Actions.text = ("They now have $" + landed.money);
		} else {
			Actions.text = ("Not enough money to buy " + unownedSpace.title + "!");
		}
	}

	public void payRent (Owner landed, Property ownedSpace) //pay rent from piece to space owner
	{
		int rentOwed = 0;
		if (ownedSpace.title == "Electric Company") {
			bool ownsWW = false;
			foreach (Property p in ownedSpace.player.properties) {
				if (p.title == "Water Works") {
					ownsWW = true;
				}
			}
			if (ownsWW) {
				rentOwed = (10 * this.diceTotal);
			}
		} else if (ownedSpace.title == "Water Works") {
			bool ownsEC = false;
			foreach (Property p in ownedSpace.player.properties) {
				if (p.title == "Electric Company") {
					ownsEC = true;
				}
			}
			if (ownsEC) {
				rentOwed = (10 * this.diceTotal);
			}
		} else {
			rentOwed = ownedSpace.rents [ownedSpace.buildings];
		}
		landed.money = landed.money - rentOwed;
		ownedSpace.player.money = ownedSpace.player.money + rentOwed;
		Actions.text = ("Player " + landed.ID + " pays Player " + ownedSpace.player.ID + " $" + rentOwed + " and now has $" + landed.money + ". Player " + ownedSpace.player.ID + " now has $" + ownedSpace.player.money);
		if (landed.money < 0) {
			Actions.text = ("Player " + landed.ID + " loses the game");
			gameOver = true;
		}
	}

	void Start ()
	{
		Actions.text = ("Game Start");
		GameBoard = new Board ();
		unityBuildings = new Image[40, 5] { {null,null,null,null,null}, {house1_1, house1_2, house1_3, house1_4, hotel1}, {null,null,null,null,null}, {house3_1, house3_2, house3_3, house3_4, hotel3}, {null,null,null,null,null}, {null,null,null,null,null}, {house6_1, house6_2, house6_3, house6_4, hotel6}, {null,null,null,null,null}, {house8_1, house8_2, house8_3, house8_4, hotel8}, {house9_1, house9_2, house9_3, house9_4, hotel9}, {null,null,null,null,null}, {house11_1, house11_2, house11_3, house11_4, hotel11}, {null,null,null,null,null}, {house13_1, house13_2, house13_3, house13_4, hotel13}, {house14_1, house14_2, house14_3, house14_4, hotel14}, {null,null,null,null,null}, {house16_1, house16_2, house16_3, house16_4, hotel16}, {null,null,null,null,null}, {house18_1, house18_2, house18_3, house18_4, hotel18}, {house19_1, house19_2, house19_3, house19_4, hotel19}, {null,null,null,null,null}, {house21_1, house21_2, house21_3, house21_4, hotel21}, {null,null,null,null,null}, {house23_1, house23_2, house23_3, house23_4, hotel23}, {house24_1, house24_2, house24_3, house24_4, hotel24}, {null,null,null,null,null}, {house26_1, house26_2, house26_3, house26_4, hotel26}, {house27_1, house27_2, house27_3, house27_4, hotel27}, {null,null,null,null,null}, {house29_1, house29_2, house29_3, house29_4, hotel29}, {null,null,null,null,null}, {house31_1, house31_2, house31_3, house31_4, hotel31}, {house32_1, house32_2, house32_3, house32_4, hotel32}, {null,null,null,null,null}, {house34_1, house34_2, house34_3, house34_4, hotel34}, {null,null,null,null,null}, {null,null,null,null,null}, {house37_1, house37_2, house37_3, house37_4, hotel37}, {null,null,null,null,null}, {house39_1, house39_2, house39_3, house39_4, hotel39} };
		//start the game
		turncounter = 0;
		unityPieces.Add (ShoePiece);
		unityPieces.Add (DogPiece);
		RentOkButton.interactable = false;
		Roll.interactable = false;
		PropertyNo.interactable = false;
		PropertyYes.interactable = false;
		GameBoard.state = 0;
	}

	void Update ()
	{
		if (!gameOver) {
			int playerID = (turncounter % 2);
			Piece currPlayer = ((Piece)GameBoard.pieces [playerID]);
			UnityPieceImage currPlayerUnity = ((UnityPieceImage)this.unityPieces [playerID]);
			if (playerID == 0) { //if player's turn
				if (GameBoard.state == 0) {
					RentOkButton.interactable = false;
					Roll.interactable = true;
					PropertyNo.interactable = false;
					PropertyYes.interactable = false;
					goto end;
				}	//reset everything to false
				else if (GameBoard.state == 1) {
					Dice (currPlayer, currPlayerUnity); //roll and move
					GameBoard.state = 5;
				}
				Tile thisTile = (Tile)GameBoard.tiles [currPlayer.location];
				if (thisTile.isProperty) { //is property
					if (thisTile.property.player != null) { //owned
						if (thisTile.property.player.ID != currPlayer.player.ID) {
							if (GameBoard.state == 5) {
								RentOkButton.interactable = true;
								Roll.interactable = false;
								PropertyNo.interactable = false;
								PropertyYes.interactable = false;
								goto end;
							}
							if (GameBoard.state == 2) {
								Debug.Log ("paying");
								payRent (currPlayer.player, thisTile.property);
								GameBoard.state = 0;
							}
						} //else owned by you
					} else { //unowned
						if (!(GameBoard.state == 4)) {
							GameBoard.state = 3;
						}
						if (GameBoard.state == 3) {
							RentOkButton.interactable = false;
							Roll.interactable = false;
							PropertyNo.interactable = true;
							PropertyYes.interactable = true;
							goto end;
						}
						if (GameBoard.state == 4) {
							purchase (currPlayer.player, thisTile.property);//buy tile //now check for color set
							bool colorset = true; //for each tile, if owner is different (or null) && colorgroup is the same, colorset false
							foreach (Tile T in GameBoard.tiles) {
								if (T.isProperty) {
									if (String.Compare (T.property.colorGroup, thisTile.property.colorGroup, false) == 1) {
										if (T.property.player == null || T.property.player.ID != currPlayer.player.ID) {
											colorset = false;
											break;
										}
									}
								}
							}
							if (colorset) {
								foreach (Tile V in GameBoard.tiles) {
									if (V.isProperty) {
										if (V.property.colorGroup == thisTile.property.colorGroup) {
											V.property.addBuilding ();
										}
									}
								}
							}
							GameBoard.state = 0;
						}
					}
				} else { //not property
					GameBoard.state = 0;
					if (thisTile.rents [0] != 0 && currPlayer.location != 0) { //assuming action tile, only true if go or tax
						currPlayer.player.money = currPlayer.player.money - thisTile.rents [0];
						Actions.text = ("Player " + currPlayer.player.ID + " loses $" + thisTile.rents [0] + " on " + thisTile.title);
					} else if (currPlayer.location == 30) { //go to jail
						currPlayer.location = 10;
						currPlayerUnity.MoveTo (currPlayer.location);
						currPlayer.isJailed = true;
						Actions.text = ("Player " + currPlayer.player.ID + " pays $50 in bail"); //will be handled later in the isJailed condition
					}
				}
			} else {	//else not player's turn
				//run above things
					Dice (currPlayer, currPlayerUnity); //roll and move
					Tile thisTile = (Tile)GameBoard.tiles [currPlayer.location];
					if (thisTile.isProperty) { //is property
						if (thisTile.property.player != null) { //owned
							if (thisTile.property.player.ID != currPlayer.player.ID) {
								payRent (currPlayer.player, thisTile.property);
							} else if (thisTile.property.buildings > 0) { //if has color set, color set is buildings = 1
								int buildingCost = (((currPlayer.location / 10) + 1) * 50);
								if (currPlayer.player.money > buildingCost && thisTile.property.buildings < 6) {
									thisTile.property.addBuilding ();
									currPlayer.player.money = currPlayer.player.money - buildingCost;
								}
								if (thisTile.property.buildings < 6) {
									Actions.text = ("Player " + currPlayer.player.ID + " has purchased a house on " + thisTile.title + ". There are now " + (thisTile.property.buildings - 1) + " houses on this property.");
								} else {
									Actions.text = ("Player " + currPlayer.player.ID + " has purchased a hotel on " + thisTile.title + ".");
								}
							}
						} else { //unowned
							purchase (currPlayer.player, thisTile.property);//buy tile
							//now check for color set
							bool colorset = true;
							//for each tile, if owner is different (or null) && colorgroup is the same, colorset false
							foreach (Tile T in GameBoard.tiles) {
								if (T.isProperty) {
									if (String.Compare (T.property.colorGroup, thisTile.property.colorGroup, false) == 1) {
										if (T.property.player == null || T.property.player.ID != currPlayer.player.ID) {
											colorset = false;
											break;
										}
									}
								}
							}
							if (colorset) {
								foreach (Tile V in GameBoard.tiles) {
									if (V.isProperty) {
										if (V.property.colorGroup == thisTile.property.colorGroup) {
											V.property.addBuilding ();
										}
									}
								}
							}
						}
					} else { //not property
						if (thisTile.rents [0] != 0 && currPlayer.location != 0) { //assuming action tile, only true if go or tax
							currPlayer.player.money = currPlayer.player.money - thisTile.rents [0];
							Actions.text = ("Player " + currPlayer.player.ID + " loses $" + thisTile.rents [0] + " on " + thisTile.title);
						} else if (currPlayer.location == 30) { //go to jail
							currPlayer.location = 10;
							currPlayerUnity.MoveTo (currPlayer.location);
							currPlayer.isJailed = true;
							Actions.text = ("Player " + currPlayer.player.ID + " pays $50 in bail"); //will be handled later in the isJailed condition
						}
					}
			}
			//update printouts
			Balances.text = "Balances:\n";
			foreach (Piece p in GameBoard.pieces) {
				Balances.text = Balances.text + "Player " + p.player.ID + ": " + p.player.money + "\n";
			}
			Balances.text = Balances.text + "\nProperties: \n";
			foreach (Tile t in GameBoard.tiles) {
				if (t.isProperty) {
					Balances.text = Balances.text + t.title + ": ";
					if (t.property.player != null) {
						Balances.text = Balances.text + "Player " + t.property.player.ID;
					}
					Balances.text = Balances.text + "\n";
				}
			}
			if (currPlayer.doubcount == 0) { //end of turn, not in jail
				turncounter++;
			}
			if (currPlayer.player.money < 0) {//game end condition
				gameOver = true;
				Actions.text = ("Game over; Player " + currPlayer.player.ID + " has $" + currPlayer.player.money);
			}
			end:;
			System.Threading.Thread.Sleep(500);
		}
	}
}

public class Board
{
	public ArrayList tiles = new ArrayList ();
	public ArrayList pieces = new ArrayList ();
	public ArrayList chanceCards = new ArrayList ();
	public ArrayList commChests = new ArrayList ();	
	public int state;																																//bottom done																										//left done																												//top done
	public int[,] Unitycoordinates = new int[40,2] { {150, -150}, {120, -150}, {90, -150}, {60, -150}, {30, -150}, {0, -150}, {-30, -150}, {-60, -150}, {-90, -150}, {-120, -150}, {-150, -150}, {-150, -120}, {-150, -90}, {-150, -60}, {-150, -30}, {-150, 0}, {-150, 30}, {-150, 60}, {-150, 90}, {-150, 120}, {-150, 150}, {-120, 150}, {-90, 150}, {-60, 150}, {-30, 150}, {0, 150}, {30, 150}, {60, 150}, {90, 150}, {120, 150}, {150, 150}, {150, 120}, {150, 90}, {150, 60}, {150, 30}, {150, 0}, {150, -30}, {150, -60}, {150, -90}, {150, -120} };
	public int getUnityCoords (int loc, int xy)
	{
		return Unitycoordinates [loc, xy];
	}

	public Board ()
	{
		pieces.Add (new Piece ("Racecar", "Red", 0));
		pieces.Add (new Piece ("Tophat", "Black", 1));
		int[] values = new int[40] {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400};
		string[] spaces = new string[40]{"Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"};
		int[] mortgages = new int[40] {0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200};
		string[] colorGroups = new string[40]{null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"};
		int[,] rents = new int[40, 7] { {-200, -200, -200, -200, -200, -200, -200}, {2, 4, 10, 30, 90, 160, 250}, {0, 0, 0, 0, 0, 0, 0}, {4, 8, 20, 60, 180, 320, 450}, {200, 200, 200, 200, 200, 200, 200}, {25, 50, 100, 200, 200, 200, 200}, {6, 12, 30, 90, 270, 400, 550}, {0, 0, 0, 0, 0, 0, 0}, {6, 12, 30, 90, 270, 400, 550}, {8, 16, 40, 100, 300, 450, 600}, {0, 0, 0, 0, 0, 0, 0}, {10, 20, 50, 150, 450, 625, 750}, {0, 0, 0, 0, 0, 0, 0}, {10, 20, 50, 150, 450, 625, 750}, {12, 24, 60, 180, 500, 700, 900}, {25, 50, 100, 200, 200, 200, 200}, {14, 28, 70, 200, 550, 750, 950}, {0, 0, 0, 0, 0, 0, 0}, {14, 28, 70, 200, 550, 750, 950}, {16, 32, 80, 220, 600, 800, 1000}, {0, 0, 0, 0, 0, 0, 0}, {18, 36, 90, 250, 700, 875, 1050}, {0, 0, 0, 0, 0, 0, 0}, {18, 36, 90, 250, 700, 875, 1050}, {20, 40, 100, 300, 750, 925, 1100}, {25, 50, 100, 200, 200, 200, 200}, {22, 44, 110, 330, 800, 975, 1150}, {22, 44, 110, 330, 800, 975, 1150}, {0, 0, 0, 0, 0, 0, 0}, {24, 48, 120, 360, 850, 1025, 1200}, {0, 0, 0, 0, 0, 0, 0}, {26, 52, 130, 390, 900, 1100, 1275}, {26, 52, 130, 390, 900, 1100, 1275}, {0, 0, 0, 0, 0, 0, 0}, {28, 52, 150, 450, 1000, 1200, 1400}, {25, 50, 100, 200, 200, 200, 200}, {0, 0, 0, 0, 0, 0, 0}, {35, 70, 175, 500, 1100, 1300, 1500}, {100, 100, 100, 100, 100, 100, 100}, {50, 100, 200, 600, 1400, 1700, 2000} };			for (int i = 0; i < 40; i++) {
			int[] passRents = new int[7];
			for (int jj = 0; jj < 7; jj++) {
				passRents [jj] = rents [i, jj];
				Debug.Log ("PassRents[" + jj + "] for " + spaces [i] + " is " + passRents [jj]);
			}
			tiles.Add (new Tile (spaces [i], values [i], passRents, mortgages [i], colorGroups [i]));
		}
		Debug.Log ("BOARD COMPLETE");
	}
}

public class Piece
{
	public Owner player;
	public string type;
	public int doubcount;
	public bool isJailed;
	public int location;

	public Piece (string in_type, string color, int id)
	{
		player = new Owner (color, id);
		type = in_type;
		doubcount = 0;
		isJailed = false;
		location = 0;
		Debug.Log (type + " created");
	}
}

public class Tile
{
	public string title;
	public Property property;
	public bool isProperty;
	public int[] rents;

	public Tile (string in_title, int value, int[] in_rents, int mortgage, string colorGroup)
	{
		title = in_title;
		rents = in_rents;
		Debug.Log (title + " created");
		if (value == 0) {
			isProperty = false;
		} else {
			isProperty = true;
			property = new Property (title, value, in_rents, mortgage, colorGroup);
		}
	}
}

public class Owner
{
	public int ID;
	public ArrayList properties = new ArrayList ();
	public int money;
	public bool hasFreeEscape;
	public string color;

	public Owner (string in_color, int in_id)
	{
		ID = in_id;
		money = 1500;
		hasFreeEscape = false;
		color = in_color;
		Debug.Log ("Player " + ID + " created");
	}
}

public class Property
{
	public int mortgage;
	public bool isMortgaged;
	public int value;
	public int buildings;
	public int[] rents = new int[6];
	public string colorGroup;
	public Owner player;
	public string title;

	public Property (string in_title, int in_value, int[] in_rents, int in_mortgage, string in_colorGroup)
	{
		mortgage = in_mortgage;
		value = in_value;
		isMortgaged = false;
		buildings = 0;
		rents = in_rents;
		colorGroup = in_colorGroup;
		player = null; //not yet owned
		Debug.Log ("property created");
		title = in_title;
	}

	public void setOwned (Owner cplayer)
	{
		player = cplayer;
	}

	public void addBuilding ()
	{
		buildings++;
	}
}

public class Card
{
	public bool type;
	//comm chest or chance
	public string text;
	public Action command;

	public Card (bool in_type, string in_text, bool m, int value)
	{
		type = in_type;
		text = in_text;
		command = new Action (m, value);
	}
}

public class Action
{
	public bool m;
	//money or motion
	public int value;

	public Action (bool in_m, int in_value)
	{
		m = in_m;
		value = in_value;
	}
}
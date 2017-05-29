using System;
using System.Collections;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;
using Random = System.Random;

public class GamePlay : MonoBehaviour {

	public Transform Target;	//where a piece is going
    public float Speed;			//how fast
	public TextMesh Balances;
    public TextMesh BalancesBoard;
	public TextMesh Actions;
	public UnityPieceImage PurplePiece, OrangePiece;
	public BuildingHandler Hotel1, Hotel3, Hotel6, Hotel8, Hotel9, Hotel11, Hotel13, Hotel14, Hotel16, Hotel18, Hotel19, Hotel21, Hotel23, Hotel24, Hotel26, Hotel27, Hotel29, Hotel31, Hotel32, Hotel34, Hotel37, Hotel39;
	public BuildingHandler House11, House12, House13, House14, House31, House32, House33, House34, House61, House62, House63, House64, House81, House82, House83, House84, House91, House92, House93, House94;
	public BuildingHandler House111, House112, House113, House114, House131, House132, House133, House134, House141, House142, House143, House144, House161, House162, House163, House164, House181, House182, House183, House184, House191, House192, House193, House194;
	public BuildingHandler House211, House212, House213, House214, House231, House232, House233, House234, House241, House242, House243, House244, House261, House262, House263, House264, House271, House272, House273, House274, House291, House292, House293, House294;
	public BuildingHandler House311, House312, House313, House314, House321, House322, House323, House324, House341, House342, House343, House344, House371, House372, House373, House374, House391, House392, House393, House394;
	public BuildingHandler Box0, Box1, Box2, Box3, Box4, Box5, Box6, Box7, Box8, Box9, Box10, Box11, Box12, Box13, Box14, Box15, Box16, Box17, Box18, Box19, Box20, Box21, Box22, Box23, Box24, Box25, Box26, Box27, Box28, Box29, Box30, Box31, Box32, Box33, Box34, Box35, Box36, Box37, Box38, Box39;
	public static Board GameBoard;
	public int DiceTotal;
	public bool GameOver;
	public ArrayList UnityPieces = new ArrayList ();
	public BuildingHandler[,] UnityBuildings;
	public BuildingHandler[] GreyBoxes;
	public Button RentOkButton;
	public Button PropertyYes;
	public Button PropertyNo;
	public Button Roll;
    public Button BuyBuilding;
    public Button Cancel;
    public int Turncounter;
    public Camera OvrPlayerController;
    public Camera BirdsEyeCam;
    public int D1;
    public int D2;
    public int LastLoc;
    public bool Unbought;
    public GameObject Ball;

    public void TurnOff(Button buttonIn)
    {
        buttonIn.enabled = false;
        var textLr = buttonIn.GetComponent<TextMesh>();
        textLr.color = Color.clear;
    }

    public void TurnOn(Button buttonIn)
    {
        buttonIn.enabled = true;
        var textLr = buttonIn.GetComponent<TextMesh>();
        textLr.color = Color.black;
    }

    public void AllOff()
    {
        TurnOff(Roll);
        TurnOff(RentOkButton);
        TurnOff(PropertyYes);
        TurnOff(PropertyNo);
        TurnOff(BuyBuilding);
        TurnOff(Cancel);
    }

    public void BluetoothWrite(Piece currPlayer)
    {
        var bluetoothFile = new StreamWriter("C:\\VirtualRealty\\BluetoothRead\\location.txt");
        var robotString = currPlayer.Player.Id.ToString();
        int distance;
        if (LastLoc < currPlayer.Location)
            distance = currPlayer.Location - LastLoc;
        else
            distance = 40 - LastLoc + currPlayer.Location;
        string distString;
        if (distance < 10)
            distString = "0" + distance;
        else
            distString = distance.ToString();
        robotString = robotString + distString;
        var cornerDist = 10 - LastLoc % 10;
        string cornerDistString;
        if (cornerDist < 10)
            cornerDistString = "0" + cornerDist;
        else
            cornerDistString = cornerDist.ToString();
        robotString = robotString + cornerDistString;
        bluetoothFile.WriteLine(robotString);
        bluetoothFile.Close();
    }

	public void Dice (Piece currPlayer, UnityPieceImage currPlayerUnity)	//probably an int later when taking care of front end
	{
		var rnd = new Random ((int)DateTime.Now.Ticks);
		D1 = rnd.Next (1, 7);
		D2 = rnd.Next (1, 7);
		DiceTotal = D1 + D2;
		if (D1 == D2) {
			currPlayer.Doubcount = currPlayer.Doubcount + 1;
			if (currPlayer.Doubcount == 3) {
                LastLoc = currPlayer.Location;
				currPlayer.Location = 10;
                BluetoothWrite(currPlayer);
				currPlayerUnity.MoveTo (currPlayer.Location, currPlayer.Player.Id);
                currPlayer.IsJailed = true;
				currPlayer.Doubcount = 0;
				currPlayer.Player.Money = currPlayer.Player.Money - 50; //bail immediate for the time being
			} else {
				if (currPlayer.Location + DiceTotal > 39)
					currPlayer.Player.Money = currPlayer.Player.Money + 200;
                LastLoc = currPlayer.Location;
				currPlayer.Location = (currPlayer.Location + DiceTotal) % 40;
                BluetoothWrite(currPlayer);
                currPlayerUnity.MoveTo (currPlayer.Location, currPlayer.Player.Id);
			}

		} else {
			if (currPlayer.Location + DiceTotal > 39)
				currPlayer.Player.Money = currPlayer.Player.Money + 200;
            LastLoc = currPlayer.Location;
			currPlayer.Location = (currPlayer.Location + DiceTotal) % 40;
            BluetoothWrite(currPlayer);
            currPlayerUnity.MoveTo (currPlayer.Location, currPlayer.Player.Id);
			currPlayer.Doubcount = 0;
		}
    }

	public void Purchase (Owner landed, Property unownedSpace)
	{
		var origMoney = landed.Money;
		if (landed.Money >= unownedSpace.Value) {
			landed.Money = landed.Money - unownedSpace.Value;
			unownedSpace.SetOwned (landed);
			landed.Properties.Add (unownedSpace);
			Actions.text = "Player " + landed.Id + " pays $" + unownedSpace.Value + " from $" + origMoney + " to purchase " + unownedSpace.Title + ". They now have $" + landed.Money + ".";
		}
	}

	public void PayRent (Owner landed, Property ownedSpace) //pay rent from piece to space owner
	{
		var rentOwed = 0;
		switch (ownedSpace.Title)
		{
			case "Electric Company":
				var ownsWw = false;
				foreach (Property p in ownedSpace.Player.Properties) {
					if (p.Title == "Water Works")
						ownsWw = true;
					else rentOwed = 4 * DiceTotal;
				}
				if (ownsWw)
					rentOwed = 10 * DiceTotal;
				break;
			case "Water Works":
				var ownsEc = false;
				foreach (Property p in ownedSpace.Player.Properties) {
					if (p.Title == "Electric Company")
						ownsEc = true;
				}
				if (ownsEc)
					rentOwed = 10 * DiceTotal;
				else rentOwed = 4 * DiceTotal;
				break;
			default:
				rentOwed = ownedSpace.Rents [ownedSpace.Buildings];
				break;
		}
		landed.Money = landed.Money - rentOwed;
		ownedSpace.Player.Money = ownedSpace.Player.Money + rentOwed;
		if (landed.Money < 0)
			GameOver = true;
	}

	private void Start ()
	{
        Debug.Log("Game Start");
		GameBoard = new Board ();
		UnityBuildings = new[,] { {null,null,null,null,null}, {House11, House12, House13, House14, Hotel1}, {null,null,null,null,null}, {House31, House32, House33, House34, Hotel3}, {null,null,null,null,null}, {null,null,null,null,null}, {House61, House62, House63, House64, Hotel6}, {null,null,null,null,null}, {House81, House82, House83, House84, Hotel8}, {House91, House92, House93, House94, Hotel9}, {null,null,null,null,null}, {House111, House112, House113, House114, Hotel11}, {null,null,null,null,null}, {House131, House132, House133, House134, Hotel13}, {House141, House142, House143, House144, Hotel14}, {null,null,null,null,null}, {House161, House162, House163, House164, Hotel16}, {null,null,null,null,null}, {House181, House182, House183, House184, Hotel18}, {House191, House192, House193, House194, Hotel19}, {null,null,null,null,null}, {House211, House212, House213, House214, Hotel21}, {null,null,null,null,null}, {House231, House232, House233, House234, Hotel23}, {House241, House242, House243, House244, Hotel24}, {null,null,null,null,null}, {House261, House262, House263, House264, Hotel26}, {House271, House272, House273, House274, Hotel27}, {null,null,null,null,null}, {House291, House292, House293, House294, Hotel29}, {null,null,null,null,null}, {House311, House312, House313, House314, Hotel31}, {House321, House322, House323, House324, Hotel32}, {null,null,null,null,null}, {House341, House342, House343, House344, Hotel34}, {null,null,null,null,null}, {null,null,null,null,null}, {House371, House372, House373, House374, Hotel37}, {null,null,null,null,null}, {House391, House392, House393, House394, Hotel39} };
		GreyBoxes = new[] { Box0, Box1, Box2, Box3, Box4, Box5, Box6, Box7, Box8, Box9, Box10, Box11, Box12, Box13, Box14, Box15, Box16, Box17, Box18, Box19, Box20, Box21, Box22, Box23, Box24, Box25, Box26, Box27, Box28, Box29, Box30, Box31, Box32, Box33, Box34, Box35, Box36, Box37, Box38, Box39 };
		var j = 0;
		foreach (var i in UnityBuildings)
		{
			if (j < 40)
			{
				GreyBoxes[j].MoveDown();
				j++;
			}
			if (i != null)
				i.MoveDown();
		}
		//start the game
		Turncounter = 0;
		UnityPieces.Add (PurplePiece);
		UnityPieces.Add (OrangePiece);
        AllOff();
        OvrPlayerController.enabled = false;
        BalancesBoard.GetComponent<MeshRenderer>().enabled = false;
        Ball.GetComponent<MeshRenderer>().enabled = true;
        GameBoard.State = 0;
        PurplePiece.MoveTo(0, 0);
        OrangePiece.MoveTo(0, 1);
	}

	private void UpdatePrints ()
	{
		Balances.text = "Balances:\n";
		foreach (Piece p in GameBoard.Pieces) {
			if (!p.Player.IsHuman)
				Balances.text = Balances.text + "CPU " + p.Player.Id + ": " + p.Player.Money + "\n";
			else
				Balances.text = Balances.text + "You: " + p.Player.Money + "\n";
		}
		BalancesBoard.text = Balances.text;
		Balances.text = Balances.text + "\nProperties: \n";
		foreach (Tile t in GameBoard.Tiles) {
			if (t.IsProperty) {
				Balances.text = Balances.text + t.Title + ": ";
				if (t.Property.Player != null) {
					if (!t.Property.Player.IsHuman)
						Balances.text = Balances.text + "CPU " + t.Property.Player.Id;
					else
						Balances.text = Balances.text + "You";
                }
				Balances.text = Balances.text + "\n";
            }
		}
	}

	private void EnableBuildings (int location, Tile thisTile)
	{
		if (thisTile.Property.Buildings > 1 && thisTile.Property.Buildings < 6)
			UnityBuildings[location, thisTile.Property.Buildings - 2].MoveUp();
        else if (thisTile.Property.Buildings == 6) {
			for (var i = 0; i < 4; i++)
				UnityBuildings [location, i].MoveDown();
			UnityBuildings [location, 4].MoveUp();
        }
	}

	private void Update()
    {
        if (Input.GetKeyDown(KeyCode.LeftShift))
        {
            BirdsEyeCam.enabled = !BirdsEyeCam.enabled;
            OvrPlayerController.enabled = !OvrPlayerController.enabled;
            if (BirdsEyeCam.enabled)
            {
                BalancesBoard.GetComponent<MeshRenderer>().enabled = false;
                Ball.GetComponent<MeshRenderer>().enabled = true;
            }
            else
            {
                BalancesBoard.GetComponent<MeshRenderer>().enabled = true;
                Ball.GetComponent<MeshRenderer>().enabled = false;
            }
        }
        UpdatePrints();
        if (!GameOver)
        {
            var playerId = Turncounter % 2;
            var currPlayer = (Piece)GameBoard.Pieces[playerId];
            var currPlayerUnity = (UnityPieceImage)UnityPieces[playerId];
            if (playerId == 0)
            { //if player's turn
                if (currPlayer.Player.HasMonopoly && GameBoard.State != 6 && GameBoard.State != 7)
                {
                    TurnOn(BuyBuilding);
                    if (Input.GetKeyDown(KeyCode.B))
                    {
                        GameBoard.BackupState = GameBoard.State;
                        GameBoard.State = 6;
                    }
                }
                if (GameBoard.State == 8)
                {
                    Debug.Log("State8");
                    if (currPlayerUnity.IsFinished())
	                    GameBoard.State = D1 == D2 ? 0 : 5;
                    else
                    {
                        AllOff();
                        goto end;
                    }
                }
                if (GameBoard.State == 6)
                {
                    TurnOn(Cancel);
                    TurnOff(BuyBuilding);
                    if (Input.GetKeyDown(KeyCode.RightControl))
                        GameBoard.State = GameBoard.BackupState;
                    Actions.text = ResolveTextSize("Select a highlighted tile to buy a building on it. \n\n Press the right ctrl key to return to game.", 30);
                    var j = 0;
                    foreach (Tile t in GameBoard.Tiles)
                    {
                        if (t.IsProperty)
                        {
                            if (t.Property.Buildings > 0 && t.Property.Buildings < 6)
                            {
                                if (t.Property.Player == null || t.Property.Player.Id != currPlayer.Player.Id && !GreyBoxes[j].IsUp)
                                {
                                }
                                else if (!GreyBoxes[j].IsUp)
                                {
                                    GreyBoxes[j].MoveUp();
                                }
                            }
                            else if (!GreyBoxes[j].IsUp)
                            {
                                GreyBoxes[j].MoveUp();
                            }
                        }
                        else if (!GreyBoxes[j].IsUp)
                            GreyBoxes[j].MoveUp();
                        j++;
                    }
                    if (Input.GetMouseButtonDown(0))
                    {
                        for (var i = 0; i < 40; i++)
                        {
	                        if (Ball.transform.position.z + .2 >= GameBoard.BallCoords[i, 0] &&
	                            Ball.transform.position.z - .2 <= GameBoard.BallCoords[i, 0] &&
	                            Ball.transform.position.x - .2 <= GameBoard.BallCoords[i, 1] &&
	                            Ball.transform.position.x + .2 >= GameBoard.BallCoords[i, 1] && !GreyBoxes[i].IsUp)
	                        {
		                        GameBoard.ToAddBuilding = i;
		                        GameBoard.State = 7;
		                        break;
	                        }
                        }
                    }
                }
                else if (GameBoard.State != 7)
                {
                    TurnOff(Cancel);
                    for (var i = 0; i < 40; i++)
                    {
                        if (GreyBoxes[i].IsUp)
                            GreyBoxes[i].MoveDown();
                    }
                }
                if (GameBoard.State == 7)
                {
                    var buildingCost = (GameBoard.ToAddBuilding / 10 + 1) * 50;
                    if (currPlayer.Player.Money > buildingCost)
                    {
                        ((Tile)GameBoard.Tiles[GameBoard.ToAddBuilding]).Property.AddBuilding();
                        currPlayer.Player.Money = currPlayer.Player.Money - buildingCost;
                        EnableBuildings(GameBoard.ToAddBuilding, (Tile)GameBoard.Tiles[GameBoard.ToAddBuilding]);
                    }
                    GameBoard.State = 6;
                }
                if (GameBoard.State == 0)
                {
                    Actions.text = ResolveTextSize("It's your turn! Roll the dice! \n\n Press K to roll.", 30);
                    TurnOff(RentOkButton);
                    TurnOn(Roll);
                    TurnOff(PropertyYes);
                    TurnOff(PropertyNo);
                    if (Input.GetKeyDown(KeyCode.K))
                    {
                        GameBoard.State = 1;
                    }
                    goto end;
                }   //reset everything to false
	            if (GameBoard.State == 1)
	            {
		            Dice(currPlayer, currPlayerUnity); //roll and move
		            GameBoard.State = 8;
		            goto end;
	            }
	            var thisTile = (Tile)GameBoard.Tiles[currPlayer.Location];
                if (thisTile.IsProperty)
                { //is property
                    if (thisTile.Property.Player != null)
                    { //owned
                        if (thisTile.Property.Player.Id != currPlayer.Player.Id)
                        {
                            if (GameBoard.State == 5)
                            {
                                Actions.text = ResolveTextSize("You owe CPU " + thisTile.Property.Player.Id + " $" + thisTile.Rents[thisTile.Property.Buildings] + " for rent on " + thisTile.Title + ". \n\n Press I to accept.", 30);
                                TurnOn(RentOkButton);
                                TurnOff(Roll);
                                TurnOff(PropertyYes);
                                TurnOff(PropertyNo);
                                if (Input.GetKeyDown(KeyCode.I))
                                {
                                    GameBoard.State = 2;
                                }
                                goto end;
                            }
                            if (GameBoard.State == 2)
                            {
                                PayRent(currPlayer.Player, thisTile.Property);
                                GameBoard.State = 0;
                            }
                        }
                        else
                        { //else owned by you
                            if (GameBoard.State != 6 && GameBoard.State != 7)
                                GameBoard.State = 0;
                        }
                    }
                    else
                    { //unowned
                        if (GameBoard.State != 4)
                            GameBoard.State = 3;
                        if (GameBoard.State == 3)
                        {
                            Actions.text = ResolveTextSize("Would you like to purchase " + thisTile.Title + " for $" + thisTile.Property.Value + "? This would leave you with $" + (currPlayer.Player.Money - thisTile.Property.Value) + ".\n\n Press J for yes or L for no.", 30);
                            TurnOff(Roll);
                            TurnOff(RentOkButton);
                            TurnOn(PropertyYes);
                            TurnOn(PropertyNo);
                            if (Input.GetKeyDown(KeyCode.L))
                            {
                                Unbought = true;
                                GameBoard.State = 4;
                            }
                            if (Input.GetKeyDown(KeyCode.J))
                            {
                                Unbought = false;
                                GameBoard.State = 4;
                            }
                            goto end;
                        }
                        if (GameBoard.State == 4)
                        {
                            if (!Unbought)
                            {
                                Purchase(currPlayer.Player, thisTile.Property);//buy tile //now check for color set
                                var colorset = true; //for each tile, if owner is different (or null) && colorgroup is the same, colorset false
                                if (string.CompareOrdinal(thisTile.Property.ColorGroup, "UTIL") != 0)
                                {
	                                if ((from Tile T in GameBoard.Tiles where T.IsProperty where string.CompareOrdinal(T.Property.ColorGroup, thisTile.Property.ColorGroup) == 0 select T).Any(T => T.Property.Player == null || T.Property.Player.Id != currPlayer.Player.Id))
		                                colorset = false;
                                }
                                else
                                {
                                    colorset = false;
                                }
                                if (colorset)
                                {
                                    currPlayer.Player.HasMonopoly = true;
                                    foreach (Tile v in GameBoard.Tiles)
                                    {
                                        if (v.IsProperty && v.Property.ColorGroup == thisTile.Property.ColorGroup)
                                            v.Property.AddBuilding();
                                    }
                                }
                            }
                            Unbought = false;
                            GameBoard.State = 0;
                        }
                    }
                }
                else
                { //not property
                    if (GameBoard.State != 6 && GameBoard.State != 7)
                        GameBoard.State = 0;
                    if (thisTile.Rents[0] != 0 && currPlayer.Location != 0)
                        currPlayer.Player.Money = currPlayer.Player.Money - thisTile.Rents[0]; //assuming action tile, only true if go or tax
                    else if (currPlayer.Location == 30)
                    { //go to jail
                        LastLoc = currPlayer.Location;
                        currPlayer.Location = 10;
                        BluetoothWrite(currPlayer);
                        currPlayerUnity.MoveTo(currPlayer.Location, currPlayer.Player.Id);
                        currPlayer.IsJailed = true;
                    }
                }
            }
            else
            {   //else not player's turn
                AllOff();
                if (currPlayerUnity.IsFinished())
                {
                    //run above things
                    Dice(currPlayer, currPlayerUnity); //roll and move
                    var thisTile = (Tile)GameBoard.Tiles[currPlayer.Location];
                    if (thisTile.IsProperty)
                    { //is property
                        if (thisTile.Property.Player != null)
                        { //owned
                            if (thisTile.Property.Player.Id != currPlayer.Player.Id)
                            {
                                PayRent(currPlayer.Player, thisTile.Property);
                            }
                            else if (thisTile.Property.Buildings > 0)
                            { //if has color set, color set is buildings = 1
                                var buildingCost = (currPlayer.Location / 10 + 1) * 50;
                                if (currPlayer.Player.Money > buildingCost && thisTile.Property.Buildings < 6)
                                {
                                    thisTile.Property.AddBuilding();
                                    currPlayer.Player.Money = currPlayer.Player.Money - buildingCost;
                                    EnableBuildings(currPlayer.Location, thisTile);
                                }
                            }
                        }
                        else
                        { //unowned
                            Purchase(currPlayer.Player, thisTile.Property);//buy tile
                                                                           //now check for color set
                            var colorset = true;
                            //for each tile, if owner is different (or null) && colorgroup is the same, colorset false
                            if (string.CompareOrdinal(thisTile.Property.ColorGroup, "UTIL") != 0)
                            {
	                            if ((from Tile T in GameBoard.Tiles where T.IsProperty where string.CompareOrdinal(T.Property.ColorGroup, thisTile.Property.ColorGroup) == 0 select T).Any(T => T.Property.Player == null || T.Property.Player.Id != currPlayer.Player.Id))
		                            colorset = false;
                            }
                            else
                                colorset = false;
                            if (colorset)
                            {
                                currPlayer.Player.HasMonopoly = true;
                                foreach (Tile v in GameBoard.Tiles)
                                {
                                    if (v.IsProperty && v.Property.ColorGroup == thisTile.Property.ColorGroup)
										v.Property.AddBuilding();
                                }
                            }
                        }
                    }
                    else
                    { //not property
                        if (thisTile.Rents[0] != 0 && currPlayer.Location != 0)
                            currPlayer.Player.Money = currPlayer.Player.Money - thisTile.Rents[0]; //assuming action tile, only true if go or tax
                        else if (currPlayer.Location == 30)
                        { //go to jail
                            LastLoc = currPlayer.Location;
                            currPlayer.Location = 10;
                            BluetoothWrite(currPlayer);
                            currPlayerUnity.MoveTo(currPlayer.Location, currPlayer.Player.Id);
                            currPlayer.IsJailed = true;
                        }
                    }
                }
            }
                //update printouts
                UpdatePrints();
                if (currPlayer.Doubcount == 0 && GameBoard.State == 0)
                    Turncounter++; //end of turn, not in jail
                if (currPlayer.Player.Money < 0)
                {
                    GameOver = true; //game end condition
	                Actions.text = !currPlayer.Player.IsHuman ? ResolveTextSize("Game over; CPU " + currPlayer.Player.Id + " has $" + currPlayer.Player.Money, 30) : ResolveTextSize("Game over; You have $" + currPlayer.Player.Money, 30);
                }
        end:;
        }
    }

 	private static string ResolveTextSize(string input, int lineLength){       
		var words = input.Split(" "[0]);
		var result = "";
		var line = "";       
		foreach(var s in words){
			var temp = line + " " + s;
			if(temp.Length > lineLength){
				result += line + "\n";
				line = s;
			}
			else {
				line = temp;
			}
		}      
		result += line;
		return result.Substring(1,result.Length-1);
	}
}

public class Board
{
    public ArrayList Tiles = new ArrayList();
    public ArrayList Pieces = new ArrayList();
    public ArrayList ChanceCards = new ArrayList();
    public ArrayList CommChests = new ArrayList();
    public int State;
    public int BackupState;
    public int ToAddBuilding;
    public double[,] BallCoords = { { -13, -3 }, { -12.3, -3 }, { -11.8, -3 }, { -11.2, -3 }, { -10.6, -3 }, { -10, -3 }, { -9.4, -3 }, { -8.8, -3 }, { -8.3, -3 }, { -7.6, -3 }, { -7, -3 }, { -6.8, -2.3 }, { -6.8, -1.8 }, { -6.8, -1.2 }, { -6.8, -0.5 }, { -6.8, 0 }, { -6.8, 0.6 }, { -6.8, 1.2 }, { -6.8, 1.8 }, { -6.8, 2.4 }, { -6.9, 3 }, { -7.7, 3.3 }, { -8.2, 3.3 }, { -8.8, 3.3 }, { -9.4, 3.3 }, { -10, 3.3 }, { -10.6, 3.3 }, { -11.2, 3.3 }, { -11.8, 3.3 }, { -12.3, 3.3 }, { -13, 3 }, { -13.3, 2.4 }, { -13.3, 1.8 }, { -13.3, 1.2 }, { -13.3, 0.6 }, { -13.3, 0 }, { -13.3, -0.6 }, { -13.3, -1.2 }, { -13.3, -1.8 }, { -13.3, -2.3 } };
    public double[,] Unitycoordinates = { { -3.32, -13.92 }, { -3.32, -13.197 }, { -3.32, -12.474 }, { -3.32, -11.751 }, { -3.32, -11.028 }, { -3.32, -10.305 }, { -3.32, -9.582 }, { -3.32, -8.859 }, { -3.32, -8.136 }, { -3.32, -7.413 }, { -3.32, -6.69 }, { -2.059, -6.276 }, { -1.336, -6.276 }, { -0.612, -6.276 }, { 0.11, -6.276 }, { 0.833, -6.276 }, { 1.556, -6.276 }, { 2.279, -6.276 }, { 3.002, -6.276 }, { 3.725, -6.276 }, { 4.448, -6.276 }, { 4.972, -7.126 }, { 4.972, -7.849 }, {4.972, -8.572 }, { 4.972, -9.295 }, { 4.972, -10.018 }, { 4.972, -10.741 }, { 4.972, -11.464 }, { 4.972, -12.187 }, { 4.972, -13.02}, { 4.972, -13.7 }, { 4, -13.7}, { 3.187, -13.7 }, { 2.464, -13.7 }, { 1.741, -13.7 }, { 1.018, -13.7 }, { 0.295, -13.7 }, {-0.428, -13.7 }, {-1.151, -13.7 }, {-1.874, -13.7 } };
	public float GetUnityCoords (int loc, int xy)
	{
		return (float)Unitycoordinates [loc, xy];
	}

	public Board ()
	{
		Pieces.Add (new Piece ("Racecar", "Red", 0));
		Pieces.Add (new Piece ("Tophat", "Black", 1));
		var values = new [] {0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400};
		var spaces = new[]{"Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"};
		int[] mortgages = {0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200};
		string[] colorGroups = {null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"};
		int[,] rents = { {-200, -200, -200, -200, -200, -200, -200}, {2, 4, 10, 30, 90, 160, 250}, {0, 0, 0, 0, 0, 0, 0}, {4, 8, 20, 60, 180, 320, 450}, {200, 200, 200, 200, 200, 200, 200}, {25, 50, 100, 200, 200, 200, 200}, {6, 12, 30, 90, 270, 400, 550}, {0, 0, 0, 0, 0, 0, 0}, {6, 12, 30, 90, 270, 400, 550}, {8, 16, 40, 100, 300, 450, 600}, {0, 0, 0, 0, 0, 0, 0}, {10, 20, 50, 150, 450, 625, 750}, {0, 0, 0, 0, 0, 0, 0}, {10, 20, 50, 150, 450, 625, 750}, {12, 24, 60, 180, 500, 700, 900}, {25, 50, 100, 200, 200, 200, 200}, {14, 28, 70, 200, 550, 750, 950}, {0, 0, 0, 0, 0, 0, 0}, {14, 28, 70, 200, 550, 750, 950}, {16, 32, 80, 220, 600, 800, 1000}, {0, 0, 0, 0, 0, 0, 0}, {18, 36, 90, 250, 700, 875, 1050}, {0, 0, 0, 0, 0, 0, 0}, {18, 36, 90, 250, 700, 875, 1050}, {20, 40, 100, 300, 750, 925, 1100}, {25, 50, 100, 200, 200, 200, 200}, {22, 44, 110, 330, 800, 975, 1150}, {22, 44, 110, 330, 800, 975, 1150}, {0, 0, 0, 0, 0, 0, 0}, {24, 48, 120, 360, 850, 1025, 1200}, {0, 0, 0, 0, 0, 0, 0}, {26, 52, 130, 390, 900, 1100, 1275}, {26, 52, 130, 390, 900, 1100, 1275}, {0, 0, 0, 0, 0, 0, 0}, {28, 52, 150, 450, 1000, 1200, 1400}, {25, 50, 100, 200, 200, 200, 200}, {0, 0, 0, 0, 0, 0, 0}, {35, 70, 175, 500, 1100, 1300, 1500}, {100, 100, 100, 100, 100, 100, 100}, {50, 100, 200, 600, 1400, 1700, 2000} };			for (int i = 0; i < 40; i++) {
			var passRents = new int[7];
			for (var jj = 0; jj < 7; jj++) {
				passRents [jj] = rents [i, jj];
				Debug.Log ("PassRents[" + jj + "] for " + spaces [i] + " is " + passRents [jj]);
			}
			Tiles.Add (new Tile (spaces [i], values [i], passRents, mortgages [i], colorGroups [i]));
		}
		Debug.Log ("BOARD COMPLETE");
	}
}

public class Piece
{
	public Owner Player;
	public string Type;
	public int Doubcount;
	public bool IsJailed;
	public int Location;

	public Piece (string inType, string color, int id)
	{
		Player = new Owner (color, id);
		Type = inType;
		Doubcount = 0;
		IsJailed = false;
		Location = 0;
		Debug.Log (Type + " created");
	}
}

public class Tile
{
	public string Title;
	public Property Property;
	public bool IsProperty;
	public int[] Rents;

	public Tile (string inTitle, int value, int[] inRents, int mortgage, string colorGroup)
	{
		Title = inTitle;
		Rents = inRents;
		Debug.Log (Title + " created");
		if (value == 0) {
			IsProperty = false;
		} else {
			IsProperty = true;
			Property = new Property (Title, value, inRents, mortgage, colorGroup);
		}
	}
}

public class Owner
{
	public int Id;
	public ArrayList Properties = new ArrayList ();
	public int Money;
	public bool HasFreeEscape;
    public bool HasMonopoly;
	public string Color;
	public bool IsHuman;

	public Owner (string inColor, int inId)
	{
		IsHuman = inId == 0;
		Id = inId;
		Money = 1500;
		HasFreeEscape = false;
        HasMonopoly = false;
		Color = inColor;
		Debug.Log ("Player " + Id + " created");
	}
}

public class Property
{
	public int Mortgage;
	public bool IsMortgaged;
	public int Value;
	public int Buildings;
	public int[] Rents;
	public string ColorGroup;
	public Owner Player;
	public string Title;

	public Property (string inTitle, int inValue, int[] inRents, int inMortgage, string inColorGroup)
	{
		Mortgage = inMortgage;
		Value = inValue;
		IsMortgaged = false;
		Buildings = 0;
		Rents = inRents;
		ColorGroup = inColorGroup;
		Player = null; //not yet owned
		Debug.Log ("property created");
		Title = inTitle;
	}

	public void SetOwned (Owner cplayer)
	{
		Player = cplayer;
	}

	public void AddBuilding ()
	{
		Buildings++;
	}
}

public class Card
{
	public bool Type;
	//comm chest or chance
	public string Text;
	public Action Command;

	public Card (bool inType, string inText, bool m, int value)
	{
		Type = inType;
		Text = inText;
		Command = new Action (m, value);
	}
}

public class Action
{
	public bool M;
	//money or motion
	public int Value;

	public Action (bool inM, int inValue)
	{
		M = inM;
		Value = inValue;
	}
}
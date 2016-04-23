using UnityEngine;
using System.Collections;
using System;
using System.IO;
using UnityEngine.UI;


public class GamePlay : MonoBehaviour {

	public Transform target;	//where a piece is going
    public float speed;			//how fast
	public TextMesh Balances;
    public TextMesh BalancesBoard;
	public TextMesh Actions;
	public UnityPieceImage PurplePiece, OrangePiece;
	public BuildingHandler hotel1, hotel3, hotel6, hotel8, hotel9, hotel11, hotel13, hotel14, hotel16, hotel18, hotel19, hotel21, hotel23, hotel24, hotel26, hotel27, hotel29, hotel31, hotel32, hotel34, hotel37, hotel39;
	public BuildingHandler house1_1, house1_2, house1_3, house1_4, house3_1, house3_2, house3_3, house3_4, house6_1, house6_2, house6_3, house6_4, house8_1, house8_2, house8_3, house8_4, house9_1, house9_2, house9_3, house9_4;
	public BuildingHandler house11_1, house11_2, house11_3, house11_4, house13_1, house13_2, house13_3, house13_4, house14_1, house14_2, house14_3, house14_4, house16_1, house16_2, house16_3, house16_4, house18_1, house18_2, house18_3, house18_4, house19_1, house19_2, house19_3, house19_4;
	public BuildingHandler house21_1, house21_2, house21_3, house21_4, house23_1, house23_2, house23_3, house23_4, house24_1, house24_2, house24_3, house24_4, house26_1, house26_2, house26_3, house26_4, house27_1, house27_2, house27_3, house27_4, house29_1, house29_2, house29_3, house29_4;
	public BuildingHandler house31_1, house31_2, house31_3, house31_4, house32_1, house32_2, house32_3, house32_4, house34_1, house34_2, house34_3, house34_4, house37_1, house37_2, house37_3, house37_4, house39_1, house39_2, house39_3, house39_4;
	public BuildingHandler Box0, Box1, Box2, Box3, Box4, Box5, Box6, Box7, Box8, Box9, Box10, Box11, Box12, Box13, Box14, Box15, Box16, Box17, Box18, Box19, Box20, Box21, Box22, Box23, Box24, Box25, Box26, Box27, Box28, Box29, Box30, Box31, Box32, Box33, Box34, Box35, Box36, Box37, Box38, Box39;
	public static Board GameBoard;
	public int diceTotal;
	public bool gameOver = false;
	public ArrayList unityPieces = new ArrayList ();
	public BuildingHandler[,] unityBuildings;
	public BuildingHandler[] greyBoxes;
	public Button RentOkButton;
	public Button PropertyYes;
	public Button PropertyNo;
	public Button Roll;
    public Button BuyBuilding;
    public Button Cancel;
    public int turncounter;
    public Camera OVRPlayerController;
    public Camera BirdsEyeCam;
    public int d1;
    public int d2;
    public int lastLoc;
    public bool unbought;
    public GameObject ball;

    public void turnOff(Button buttonIn)
    {
        buttonIn.enabled = false;
        var textLR = buttonIn.GetComponent<TextMesh>();
        textLR.color = Color.clear;
    }

    public void turnOn(Button buttonIn)
    {
        buttonIn.enabled = true;
        var textLR = buttonIn.GetComponent<TextMesh>();
        textLR.color = Color.black;
    }

    public void allOff()
    {
        turnOff(Roll);
        turnOff(RentOkButton);
        turnOff(PropertyYes);
        turnOff(PropertyNo);
        turnOff(BuyBuilding);
        turnOff(Cancel);
    }

    public void BluetoothWrite(Piece currPlayer)
    {
        StreamWriter BluetoothFile = new StreamWriter("C:\\VirtualRealty\\BluetoothRead\\location.txt");
        string RobotString = currPlayer.player.ID.ToString();
        int distance;
        if (lastLoc < currPlayer.location)
        {
            distance = currPlayer.location - lastLoc;
        }
        else
        {
            distance = 40 - lastLoc + currPlayer.location;
        }
        string dist_string;
        if (distance < 10)
        {
            dist_string = "0" + distance.ToString();
        }
        else
        {
            dist_string = distance.ToString();
        }
        RobotString = RobotString + dist_string;
        int corner_dist = (10 - (lastLoc % 10));
        string corner_dist_string;
        if (corner_dist < 10)
        {
            corner_dist_string = "0" + corner_dist.ToString();
        }
        else
        {
            corner_dist_string = corner_dist.ToString();
        }
        RobotString = RobotString + corner_dist_string;
        BluetoothFile.WriteLine(RobotString);
        BluetoothFile.Close();
    }

	public void Dice (Piece currPlayer, UnityPieceImage currPlayerUnity)	//probably an int later when taking care of front end
	{
		System.Random rnd = new System.Random ((int)DateTime.Now.Ticks);
		d1 = rnd.Next (1, 7);
		d2 = rnd.Next (1, 7);
		diceTotal = d1 + d2;
		if (d1 == d2) {
			currPlayer.doubcount = currPlayer.doubcount + 1;
			if (currPlayer.doubcount == 3) {
                lastLoc = currPlayer.location;
				currPlayer.location = 10;
                BluetoothWrite(currPlayer);
				currPlayerUnity.MoveTo (currPlayer.location, currPlayer.player.ID);
                currPlayer.isJailed = true;
				currPlayer.doubcount = 0;
				currPlayer.player.money = currPlayer.player.money - 50; //bail immediate for the time being
			} else {
				if (currPlayer.location + diceTotal > 39) {
					currPlayer.player.money = currPlayer.player.money + 200;
				}
                lastLoc = currPlayer.location;
				currPlayer.location = (currPlayer.location + diceTotal) % 40;
                BluetoothWrite(currPlayer);
                currPlayerUnity.MoveTo (currPlayer.location, currPlayer.player.ID);
			}

		} else {
			if (currPlayer.location + diceTotal > 39) {
				currPlayer.player.money = currPlayer.player.money + 200;
			}
            lastLoc = currPlayer.location;
			currPlayer.location = (currPlayer.location + diceTotal) % 40;
            BluetoothWrite(currPlayer);
            currPlayerUnity.MoveTo (currPlayer.location, currPlayer.player.ID);
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
			Actions.text = ("Player " + landed.ID + " pays $" + unownedSpace.value + " from $" + origMoney + " to purchase " + unownedSpace.title + ". They now have $" + landed.money + ".");
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
                else { rentOwed = (4 * diceTotal); }
            }
			if (ownsWW) {
				rentOwed = (10 * diceTotal);
			}
		} else if (ownedSpace.title == "Water Works") {
			bool ownsEC = false;
			foreach (Property p in ownedSpace.player.properties) {
				if (p.title == "Electric Company") {
					ownsEC = true;
				}
			}
			if (ownsEC) {
				rentOwed = (10 * diceTotal);
			}
            else { rentOwed = (4 * diceTotal); }
		} else {
			rentOwed = ownedSpace.rents [ownedSpace.buildings];
		}
		landed.money = landed.money - rentOwed;
		ownedSpace.player.money = ownedSpace.player.money + rentOwed;
		if (landed.money < 0) {
			gameOver = true;
		}
	}

	void Start ()
	{
        Debug.Log("Game Start");
		GameBoard = new Board ();
		unityBuildings = new BuildingHandler[40, 5] { {null,null,null,null,null}, {house1_1, house1_2, house1_3, house1_4, hotel1}, {null,null,null,null,null}, {house3_1, house3_2, house3_3, house3_4, hotel3}, {null,null,null,null,null}, {null,null,null,null,null}, {house6_1, house6_2, house6_3, house6_4, hotel6}, {null,null,null,null,null}, {house8_1, house8_2, house8_3, house8_4, hotel8}, {house9_1, house9_2, house9_3, house9_4, hotel9}, {null,null,null,null,null}, {house11_1, house11_2, house11_3, house11_4, hotel11}, {null,null,null,null,null}, {house13_1, house13_2, house13_3, house13_4, hotel13}, {house14_1, house14_2, house14_3, house14_4, hotel14}, {null,null,null,null,null}, {house16_1, house16_2, house16_3, house16_4, hotel16}, {null,null,null,null,null}, {house18_1, house18_2, house18_3, house18_4, hotel18}, {house19_1, house19_2, house19_3, house19_4, hotel19}, {null,null,null,null,null}, {house21_1, house21_2, house21_3, house21_4, hotel21}, {null,null,null,null,null}, {house23_1, house23_2, house23_3, house23_4, hotel23}, {house24_1, house24_2, house24_3, house24_4, hotel24}, {null,null,null,null,null}, {house26_1, house26_2, house26_3, house26_4, hotel26}, {house27_1, house27_2, house27_3, house27_4, hotel27}, {null,null,null,null,null}, {house29_1, house29_2, house29_3, house29_4, hotel29}, {null,null,null,null,null}, {house31_1, house31_2, house31_3, house31_4, hotel31}, {house32_1, house32_2, house32_3, house32_4, hotel32}, {null,null,null,null,null}, {house34_1, house34_2, house34_3, house34_4, hotel34}, {null,null,null,null,null}, {null,null,null,null,null}, {house37_1, house37_2, house37_3, house37_4, hotel37}, {null,null,null,null,null}, {house39_1, house39_2, house39_3, house39_4, hotel39} };
		greyBoxes = new BuildingHandler[40] { Box0, Box1, Box2, Box3, Box4, Box5, Box6, Box7, Box8, Box9, Box10, Box11, Box12, Box13, Box14, Box15, Box16, Box17, Box18, Box19, Box20, Box21, Box22, Box23, Box24, Box25, Box26, Box27, Box28, Box29, Box30, Box31, Box32, Box33, Box34, Box35, Box36, Box37, Box38, Box39 };
		int j = 0;
		foreach (BuildingHandler i in unityBuildings)
		{
			if (j < 40)
			{
				greyBoxes[j].MoveDown();
				j++;
			}
			if (i != null)
			{
				i.MoveDown();
			}
		}
		//start the game
		turncounter = 0;
		unityPieces.Add (PurplePiece);
		unityPieces.Add (OrangePiece);
        allOff();
        OVRPlayerController.enabled = false;
        BalancesBoard.GetComponent<MeshRenderer>().enabled = false;
        ball.GetComponent<MeshRenderer>().enabled = true;
        GameBoard.state = 0;
        PurplePiece.MoveTo(0, 0);
        OrangePiece.MoveTo(0, 1);
	}

	void updatePrints ()
	{
		Balances.text = "Balances:\n";
		foreach (Piece p in GameBoard.pieces) {
			if (!p.player.isHuman) {
				Balances.text = Balances.text + "CPU " + p.player.ID + ": " + p.player.money + "\n";
			} else {
				Balances.text = Balances.text + "You: " + p.player.money + "\n";
			}
		}
		BalancesBoard.text = Balances.text;
		Balances.text = Balances.text + "\nProperties: \n";
		foreach (Tile t in GameBoard.tiles) {
			if (t.isProperty) {
				Balances.text = Balances.text + t.title + ": ";
				if (t.property.player != null) {
					if (!t.property.player.isHuman) {
						Balances.text = Balances.text + "CPU " + t.property.player.ID;
					} else {
						Balances.text = Balances.text + "You";
					}
                }
				Balances.text = Balances.text + "\n";
            }
		}
	}

	void enableBuildings (int location, Tile thisTile)
	{
		if (thisTile.property.buildings > 1 && thisTile.property.buildings < 6) {
			unityBuildings[location, thisTile.property.buildings - 2].MoveUp();
        } else if (thisTile.property.buildings == 6) {
			for (int i = 0; i < 4; i++) {
				unityBuildings [location, i].MoveDown();
            }
			unityBuildings [location, 4].MoveUp();
        }
	}

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.LeftShift))
        {
            BirdsEyeCam.enabled = !BirdsEyeCam.enabled;
            OVRPlayerController.enabled = !OVRPlayerController.enabled;
            if (BirdsEyeCam.enabled)
            {
                BalancesBoard.GetComponent<MeshRenderer>().enabled = false;
                ball.GetComponent<MeshRenderer>().enabled = true;
            }
            else
            {
                BalancesBoard.GetComponent<MeshRenderer>().enabled = true;
                ball.GetComponent<MeshRenderer>().enabled = false;
            }
        }
        updatePrints();
        if (!gameOver)
        {
            int playerID = (turncounter % 2);
            Piece currPlayer = ((Piece)GameBoard.pieces[playerID]);
            UnityPieceImage currPlayerUnity = ((UnityPieceImage)this.unityPieces[playerID]);
            if (playerID == 0)
            { //if player's turn
                if (currPlayer.player.hasMonopoly && GameBoard.state != 6 && GameBoard.state != 7)
                {
                    turnOn(BuyBuilding);
                    if ((Input.GetKeyDown(KeyCode.B)))
                    {
                        GameBoard.backupState = GameBoard.state;
                        GameBoard.state = 6;
                    }
                }
                if (GameBoard.state == 8)
                {
                    Debug.Log("State8");
                    if (currPlayerUnity.isFinished())
                    {
                        if (d1 == d2)
                        {
                            GameBoard.state = 0;
                        }
                        else
                        {
                            GameBoard.state = 5;
                        }
                    }
                    else
                    {
                        allOff();
                        goto end;
                    }
                }
                if (GameBoard.state == 6)
                {
                    turnOn(Cancel);
                    turnOff(BuyBuilding);
                    if ((Input.GetKeyDown(KeyCode.RightControl)))
                    {
                        GameBoard.state = GameBoard.backupState;
                    }
                    Actions.text = ResolveTextSize("Select a highlighted tile to buy a building on it. \n\n Press the right ctrl key to return to game.", 30);
                    int j = 0;
                    foreach (Tile t in GameBoard.tiles)
                    {
                        if (t.isProperty)
                        {
                            if (t.property.buildings > 0 && t.property.buildings < 6)
                            {
                                if ((t.property.player != null) && (t.property.player.ID == currPlayer.player.ID))
                                {
                                }
                                else if (!greyBoxes[j].isUp)
                                {
                                    greyBoxes[j].MoveUp();
                                }
                            }
                            else if (!greyBoxes[j].isUp)
                            {
                                greyBoxes[j].MoveUp();
                            }
                        }
                        else if (!greyBoxes[j].isUp)
                        {
                            greyBoxes[j].MoveUp();
                        }
                        j++;
                    }
                    if ((Input.GetMouseButtonDown(0)))
                    {
                        for (int i = 0; i < 40; i++)
                        {
                            if (((ball.transform.position.z + .2 >= GameBoard.ballCoords[i, 0]) && (ball.transform.position.z - .2 <= GameBoard.ballCoords[i, 0])) && ((ball.transform.position.x - .2 <= GameBoard.ballCoords[i, 1]) && ((ball.transform.position.x + .2 >= GameBoard.ballCoords[i, 1]))))
                            {
                                if (!greyBoxes[i].isUp)
                                {
                                    GameBoard.toAddBuilding = i;
                                    GameBoard.state = 7;
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (GameBoard.state != 7)
                {
                    turnOff(Cancel);
                    for (int i = 0; i < 40; i++)
                    {
                        if (greyBoxes[i].isUp)
                        {
                            greyBoxes[i].MoveDown();
                        }
                    }
                }
                if (GameBoard.state == 7)
                {
                    int buildingCost = (((GameBoard.toAddBuilding / 10) + 1) * 50);
                    if (currPlayer.player.money > buildingCost)
                    {
                        ((Tile)GameBoard.tiles[GameBoard.toAddBuilding]).property.addBuilding();
                        currPlayer.player.money = currPlayer.player.money - buildingCost;
                        enableBuildings(GameBoard.toAddBuilding, ((Tile)GameBoard.tiles[GameBoard.toAddBuilding]));
                    }
                    GameBoard.state = 6;
                }
                if (GameBoard.state == 0)
                {
                    Actions.text = ResolveTextSize("It's your turn! Roll the dice! \n\n Press K to roll.", 30);
                    turnOff(RentOkButton);
                    turnOn(Roll);
                    turnOff(PropertyYes);
                    turnOff(PropertyNo);
                    if ((Input.GetKeyDown(KeyCode.K)))
                    {
                        GameBoard.state = 1;
                    }
                    goto end;
                }   //reset everything to false
                else if (GameBoard.state == 1)
                {
                    Dice(currPlayer, currPlayerUnity); //roll and move
                    GameBoard.state = 8;
                    goto end;
                }
                Tile thisTile = (Tile)GameBoard.tiles[currPlayer.location];
                if (thisTile.isProperty)
                { //is property
                    if (thisTile.property.player != null)
                    { //owned
                        if (thisTile.property.player.ID != currPlayer.player.ID)
                        {
                            if (GameBoard.state == 5)
                            {
                                Actions.text = ResolveTextSize("You owe CPU " + thisTile.property.player.ID + " $" + thisTile.rents[thisTile.property.buildings] + " for rent on " + thisTile.title + ". \n\n Press I to accept.", 30);
                                turnOn(RentOkButton);
                                turnOff(Roll);
                                turnOff(PropertyYes);
                                turnOff(PropertyNo);
                                if ((Input.GetKeyDown(KeyCode.I)))
                                {
                                    GameBoard.state = 2;
                                }
                                goto end;
                            }
                            if (GameBoard.state == 2)
                            {
                                payRent(currPlayer.player, thisTile.property);
                                GameBoard.state = 0;
                            }
                        }
                        else
                        { //else owned by you
                            if (GameBoard.state != 6 && GameBoard.state != 7)
                            {
                                GameBoard.state = 0;
                            }
                        }
                    }
                    else
                    { //unowned
                        if (!(GameBoard.state == 4))
                        {
                            GameBoard.state = 3;
                        }
                        if (GameBoard.state == 3)
                        {
                            Actions.text = ResolveTextSize("Would you like to purchase " + thisTile.title + " for $" + thisTile.property.value + "? This would leave you with $" + (currPlayer.player.money - thisTile.property.value) + ".\n\n Press J for yes or L for no.", 30);
                            turnOff(Roll);
                            turnOff(RentOkButton);
                            turnOn(PropertyYes);
                            turnOn(PropertyNo);
                            if ((Input.GetKeyDown(KeyCode.L)))
                            {
                                unbought = true;
                                GameBoard.state = 4;
                            }
                            if ((Input.GetKeyDown(KeyCode.J)))
                            {
                                unbought = false;
                                GameBoard.state = 4;
                            }
                            goto end;
                        }
                        if (GameBoard.state == 4)
                        {
                            if (!unbought)
                            {
                                purchase(currPlayer.player, thisTile.property);//buy tile //now check for color set
                                bool colorset = true; //for each tile, if owner is different (or null) && colorgroup is the same, colorset false
                                if (!(String.Compare(thisTile.property.colorGroup, "UTIL") == 0))
                                {
                                    foreach (Tile T in GameBoard.tiles)
                                    {
                                        if (T.isProperty)
                                        {
                                            if (String.Compare(T.property.colorGroup, thisTile.property.colorGroup) == 0)
                                            {
                                                if (T.property.player == null || T.property.player.ID != currPlayer.player.ID)
                                                {
                                                    colorset = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    colorset = false;
                                }
                                if (colorset)
                                {
                                    currPlayer.player.hasMonopoly = true;
                                    foreach (Tile V in GameBoard.tiles)
                                    {
                                        if (V.isProperty)
                                        {
                                            if (V.property.colorGroup == thisTile.property.colorGroup)
                                            {
                                                V.property.addBuilding();
                                            }
                                        }
                                    }
                                }
                            }
                            unbought = false;
                            GameBoard.state = 0;
                        }
                    }
                }
                else
                { //not property
                    if (GameBoard.state != 6 && GameBoard.state != 7)
                    {
                        GameBoard.state = 0;
                    }
                    if (thisTile.rents[0] != 0 && currPlayer.location != 0)
                    { //assuming action tile, only true if go or tax
                        currPlayer.player.money = currPlayer.player.money - thisTile.rents[0];
                    }
                    else if (currPlayer.location == 30)
                    { //go to jail
                        lastLoc = currPlayer.location;
                        currPlayer.location = 10;
                        BluetoothWrite(currPlayer);
                        currPlayerUnity.MoveTo(currPlayer.location, currPlayer.player.ID);
                        currPlayer.isJailed = true;
                    }
                }
            }
            else
            {   //else not player's turn
                allOff();
                if (currPlayerUnity.isFinished())
                {
                    //run above things
                    Dice(currPlayer, currPlayerUnity); //roll and move
                    Tile thisTile = (Tile)GameBoard.tiles[currPlayer.location];
                    if (thisTile.isProperty)
                    { //is property
                        if (thisTile.property.player != null)
                        { //owned
                            if (thisTile.property.player.ID != currPlayer.player.ID)
                            {
                                payRent(currPlayer.player, thisTile.property);
                            }
                            else if (thisTile.property.buildings > 0)
                            { //if has color set, color set is buildings = 1
                                int buildingCost = (((currPlayer.location / 10) + 1) * 50);
                                if (currPlayer.player.money > buildingCost && thisTile.property.buildings < 6)
                                {
                                    thisTile.property.addBuilding();
                                    currPlayer.player.money = currPlayer.player.money - buildingCost;
                                    enableBuildings(currPlayer.location, thisTile);
                                }
                            }
                        }
                        else
                        { //unowned
                            purchase(currPlayer.player, thisTile.property);//buy tile
                                                                           //now check for color set
                            bool colorset = true;
                            //for each tile, if owner is different (or null) && colorgroup is the same, colorset false
                            if (!(String.Compare(thisTile.property.colorGroup, "UTIL") == 0))
                            {
                                foreach (Tile T in GameBoard.tiles)
                                {
                                    if (T.isProperty)
                                    {
                                        if (String.Compare(T.property.colorGroup, thisTile.property.colorGroup) == 0)
                                        {
                                            if (T.property.player == null || T.property.player.ID != currPlayer.player.ID)
                                            {
                                                colorset = false;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                colorset = false;
                            }
                            if (colorset)
                            {
                                currPlayer.player.hasMonopoly = true;
                                foreach (Tile V in GameBoard.tiles)
                                {
                                    if (V.isProperty)
                                    {
                                        if (V.property.colorGroup == thisTile.property.colorGroup)
                                        {
                                            V.property.addBuilding();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    { //not property
                        if (thisTile.rents[0] != 0 && currPlayer.location != 0)
                        { //assuming action tile, only true if go or tax
                            currPlayer.player.money = currPlayer.player.money - thisTile.rents[0];
                        }
                        else if (currPlayer.location == 30)
                        { //go to jail
                            lastLoc = currPlayer.location;
                            currPlayer.location = 10;
                            BluetoothWrite(currPlayer);
                            currPlayerUnity.MoveTo(currPlayer.location, currPlayer.player.ID);
                            currPlayer.isJailed = true;
                        }
                    }
                }
            }
                //update printouts
                updatePrints();
                if (currPlayer.doubcount == 0 && GameBoard.state == 0)
                { //end of turn, not in jail
                    turncounter++;
                }
                if (currPlayer.player.money < 0)
                {//game end condition
                    gameOver = true;
                    if (!currPlayer.player.isHuman)
                    {
                        Actions.text = ResolveTextSize("Game over; CPU " + currPlayer.player.ID + " has $" + currPlayer.player.money, 30);
                    }
                    else
                    {
                        Actions.text = ResolveTextSize("Game over; You have $" + currPlayer.player.money, 30);
                    }
                }
        end:;
        }
    }

 	private string ResolveTextSize(string input, int lineLength){       
		string[] words = input.Split(" "[0]);
		string result = "";
		string line = "";       
		foreach(string s in words){
			string temp = line + " " + s;
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
    public ArrayList tiles = new ArrayList();
    public ArrayList pieces = new ArrayList();
    public ArrayList chanceCards = new ArrayList();
    public ArrayList commChests = new ArrayList();
    public int state;
    public int backupState;
    public int toAddBuilding;
    public double[,] ballCoords = new double[40, 2] { { -13, -3 }, { -12.3, -3 }, { -11.8, -3 }, { -11.2, -3 }, { -10.6, -3 }, { -10, -3 }, { -9.4, -3 }, { -8.8, -3 }, { -8.3, -3 }, { -7.6, -3 }, { -7, -3 }, { -6.8, -2.3 }, { -6.8, -1.8 }, { -6.8, -1.2 }, { -6.8, -0.5 }, { -6.8, 0 }, { -6.8, 0.6 }, { -6.8, 1.2 }, { -6.8, 1.8 }, { -6.8, 2.4 }, { -6.9, 3 }, { -7.7, 3.3 }, { -8.2, 3.3 }, { -8.8, 3.3 }, { -9.4, 3.3 }, { -10, 3.3 }, { -10.6, 3.3 }, { -11.2, 3.3 }, { -11.8, 3.3 }, { -12.3, 3.3 }, { -13, 3 }, { -13.3, 2.4 }, { -13.3, 1.8 }, { -13.3, 1.2 }, { -13.3, 0.6 }, { -13.3, 0 }, { -13.3, -0.6 }, { -13.3, -1.2 }, { -13.3, -1.8 }, { -13.3, -2.3 } };
    public double[,] Unitycoordinates = new double[40, 2] { { -3.32, -13.92 }, { -3.32, -13.197 }, { -3.32, -12.474 }, { -3.32, -11.751 }, { -3.32, -11.028 }, { -3.32, -10.305 }, { -3.32, -9.582 }, { -3.32, -8.859 }, { -3.32, -8.136 }, { -3.32, -7.413 }, { -3.32, -6.69 }, { -2.059, -6.276 }, { -1.336, -6.276 }, { -0.612, -6.276 }, { 0.11, -6.276 }, { 0.833, -6.276 }, { 1.556, -6.276 }, { 2.279, -6.276 }, { 3.002, -6.276 }, { 3.725, -6.276 }, { 4.448, -6.276 }, { 4.972, -7.126 }, { 4.972, -7.849 }, {4.972, -8.572 }, { 4.972, -9.295 }, { 4.972, -10.018 }, { 4.972, -10.741 }, { 4.972, -11.464 }, { 4.972, -12.187 }, { 4.972, -13.02}, { 4.972, -13.7 }, { 4, -13.7}, { 3.187, -13.7 }, { 2.464, -13.7 }, { 1.741, -13.7 }, { 1.018, -13.7 }, { 0.295, -13.7 }, {-0.428, -13.7 }, {-1.151, -13.7 }, {-1.874, -13.7 } };
	public float getUnityCoords (int loc, int xy)
	{
		return (float)Unitycoordinates [loc, xy];
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
    public bool hasMonopoly;
	public string color;
	public bool isHuman;

	public Owner (string in_color, int in_id)
	{
		if (in_id == 0) {
			isHuman = true;
		} else {
			isHuman = false;
		}
		ID = in_id;
		money = 1500;
		hasFreeEscape = false;
        hasMonopoly = false;
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
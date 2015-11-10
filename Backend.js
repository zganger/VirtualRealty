main();

function main()
{
    var GameBoard = new Board();
    beginGameplay(GameBoard);
}

function beginGameplay(GameBoard)
{
    var self = this;
    var turncounter = 0;
    var gameOver = false;
    while(!gameOver)
    {
        var currPlayer = GameBoard.pieces[(turncounter%2)]; //of type piece
        Dice(currPlayer, self);
        if(GameBoard.tiles[currPlayer.location].property != null) //is property
        {
           if(GameBoard.tiles[currPlayer.location].property.player != null) //owned
           {
               payRent(currPlayer.owner, GameBoard.tiles[currPlayer.location].property);
               //check if property is owned
           }
           else //unowned
           {
               purchase(currPlayer.owner, GameBoard.tiles[currPlayer.location].property);
           }
        }
        else //not a property, action tile
        {
            
        }
        //game end condition
    }
}

function payRent(landed, ownedSpace) //pay rent from piece to space owner
{
    var rentOwed = ownedSpace.rents(ownedSpace.buildings);
    landed.money = landed.money - rentOwed;
    ownedSpace.player.money = ownedSpace.player.money + rentOwed;
    console.log("payer has $" + landed.money);
    console.log("receiver has $" + ownedSpace.player.money);
    if(landed.money <= 0)
    {
        console.log("Rent payer loses");
    }
}

function purchase(landed, unownedSpace)
{
    if(landed.money >= ownedSpace.value)
    {
        landed.money = landed.money - unownedSpace.value;
        unownedSpace.setOwn(landed);
        console.log(landed.title + " now owns " + unownedSpace.title);
    }
}

function Dice(currPlayer, board)
{
    //var die1 = document.getElementById("die1");
    //var die2 = document.getElementById("die2");
    var d1 = Math.floor(Math.random() * 6) + 1;
    var d2 = Math.floor(Math.random() * 6) + 1;
    var diceTotal = d1 + d2;
    if(d1 == d2)
    {
        currPlayer.doubcount = currPlayer.doubcount + 1;
        if(currPlayer.doubcount == 3)
        {
            console.log("Three sets of doubles, go to jail");
            currPlayer.location = 10;
            currPlayer.isJailed = true;
        }
        else
        {
            currPlayer.location = (currPlayer.location + diceTotal) % 40;
            //edit position somehow
            console.log("Move forward " + diceTotal);
            console.log("Doubles, go again!");
            //adjust turn counter (use a flag?)
        }
    }
    else
    {
        currPlayer.location = (currPlayer.location + diceTotal) % 40;
        //edit position somehow
        console.log("Move forward " + diceTotal);
        //console.log("You are on " + board.spaces[currPlayer.location] + ". Pay $" + board.rent[currPlayer.location][board.tiles[currPlayer.location].);  //add houses rates
        currPlayer.doubcount = 0;
    }
    //return diceTotal;
}

var Piece
{
    function Piece(player, in_type, color, ID)
    {
        this.owner = Owner(color, ID);
        this.type = in_type;
        this.doubcount = 0;
        this.isJailed = false;
        this.location = 0; //reference using Board.tiles[location]
    }
}

var Board
{
    function Board()
    {
        this.tiles = new Array();
        var values = [0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400]; //all values correspond with title index
        var spaces = ["Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"];
        var mortgages = [0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200];
        //matrix of rents
        var rent = new Array(40);
        for(var ii = 0; ii < 40; ii++) //properties
        {
            rent[ii] = new Array(6); //buildings
        }
        rent[0][0] = -200;      //Go
        rent[0][1] = -200;      //does negative rent make sense if you always pick up???
        rent[0][2] = -200;
        rent[0][3] = -200;
        rent[0][4] = -200;
        rent[0][5] = -200;
        rent[1][0] = 2;      //Mediterranean Avenue
        rent[1][1] = 10;
        rent[1][2] = 30;
        rent[1][3] = 90;
        rent[1][4] = 160;
        rent[1][5] = 250;
        rent[2][0] = 0;     //Community Chest
        rent[2][1] = 0;
        rent[2][2] = 0;
        rent[2][3] = 0;
        rent[2][4] = 0;
        rent[2][5] = 0;
        rent[3][0] = 4;     //Baltic
        rent[3][1] = 20;
        rent[3][2] = 60;
        rent[3][3] = 180;
        rent[3][4] = 320;
        rent[3][5] = 450;
        rent[4][0] = 200;     //income tax
        rent[4][1] = 200;     //or 10%????
        rent[4][2] = 200;
        rent[4][3] = 200;
        rent[4][4] = 200;
        rent[4][5] = 200;
        rent[5][0] = 25;     //RR
        rent[5][1] = 50;
        rent[5][2] = 100;
        rent[5][3] = 200;
        rent[5][4] = 200;
        rent[5][5] = 200;
        rent[6][0] = 6;    //Oriental Avenue
        rent[6][1] = 30;
        rent[6][2] = 90;
        rent[6][3] = 270;
        rent[6][4] = 400;
        rent[6][5] = 550;
        rent[7][0] = 0;     //chance
        rent[7][1] = 0;
        rent[7][2] = 0;
        rent[7][3] = 0;
        rent[7][4] = 0;
        rent[7][5] = 0;
        rent[8][0] = 6;     //Vermont
        rent[8][1] = 30;
        rent[8][2] = 90;
        rent[8][3] = 270;
        rent[8][4] = 400;
        rent[8][5] = 550;
        rent[9][0] = 8;     //Connecticut
        rent[9][1] = 40;
        rent[9][2] = 100;
        rent[9][3] = 300;
        rent[9][4] = 450;
        rent[9][5] = 600;
        rent[10][0] = 0;    //jail
        rent[10][1] = 0;
        rent[10][2] = 0;
        rent[10][3] = 0;
        rent[10][4] = 0;
        rent[10][5] = 0;
        rent[11][0] = 10;    //st. charles place
        rent[11][1] = 50;
        rent[11][2] = 150;
        rent[11][3] = 450;
        rent[11][4] = 625;
        rent[11][5] = 750;
        rent[12][0] = 0;    //electric
        rent[12][1] = 0;    //????
        rent[12][2] = 0;
        rent[12][3] = 0;
        rent[12][4] = 0;
        rent[12][5] = 0;
        rent[13][0] = 10;    //states ave
        rent[13][1] = 50;
        rent[13][2] = 150;
        rent[13][3] = 450;
        rent[13][4] = 625;
        rent[13][5] = 750;
        rent[14][0] = 12;       //virginia ave
        rent[14][1] = 60;
        rent[14][2] = 180;
        rent[14][3] = 500;
        rent[14][4] = 700;
        rent[14][5] = 900;
        rent[15][0] = 25;     //RR
        rent[15][1] = 50;
        rent[15][2] = 100;
        rent[15][3] = 200;
        rent[15][4] = 200;
        rent[15][5] = 200;
        rent[16][0] = 14;       //st james
        rent[16][1] = 70;
        rent[16][2] = 200;
        rent[16][3] = 550;
        rent[16][4] = 750;
        rent[16][5] = 950;
        rent[17][0] = 0;    //community chest
        rent[17][1] = 0;    
        rent[17][2] = 0;
        rent[17][3] = 0;
        rent[17][4] = 0;
        rent[17][5] = 0;
        rent[18][0] = 14;       //tennesse ave
        rent[18][1] = 70;
        rent[18][2] = 200;
        rent[18][3] = 550;
        rent[18][4] = 750;
        rent[18][5] = 950;
        rent[19][0] = 16;
        rent[19][1] = 80;
        rent[19][2] = 220;
        rent[19][3] = 600;
        rent[19][4] = 800;
        rent[19][5] = 1000;
        rent[20][0] = 0;    //free parking
        rent[20][1] = 0;    
        rent[20][2] = 0;
        rent[20][3] = 0;
        rent[20][4] = 0;
        rent[20][5] = 0;
        rent[21][0] = 18;   //kentucky avenue
        rent[21][1] = 90;
        rent[21][2] = 250;
        rent[21][3] = 700;
        rent[21][4] = 875;
        rent[21][5] = 1050;
        rent[22][0] = 0;    //chance
        rent[22][1] = 0;
        rent[22][2] = 0;
        rent[22][3] = 0;
        rent[22][4] = 0;
        rent[22][5] = 0;
        rent[23][0] = 18;   //indiana avenue
        rent[23][1] = 90;
        rent[23][2] = 250;
        rent[23][3] = 700;
        rent[23][4] = 875;
        rent[23][5] = 1050;
        rent[24][0] = 20;   //illinois avenue
        rent[24][1] = 100;
        rent[24][2] = 300;
        rent[24][3] = 750;
        rent[24][4] = 925;
        rent[24][5] = 1100;
        rent[25][0] = 25;     //RR
        rent[25][1] = 50;
        rent[25][2] = 100;
        rent[25][3] = 200;
        rent[25][4] = 200;
        rent[25][5] = 200;
        rent[26][0] = 22;     //Atlantic
        rent[26][1] = 110;
        rent[26][2] = 330;
        rent[26][3] = 800;
        rent[26][4] = 975;
        rent[26][5] = 1150;
        rent[27][0] = 22;       //ventor ave
        rent[27][1] = 110;
        rent[27][2] = 330
        rent[27][3] = 800;
        rent[27][4] = 975;
        rent[27][5] = 1150;
        rent[28][0] = 0;    //water works
        rent[28][1] = 0;    //????
        rent[28][2] = 0;
        rent[28][3] = 0;
        rent[28][4] = 0;
        rent[28][5] = 0;
        rent[29][0] = 24;        //marvin gardens
        rent[29][1] = 120;
        rent[29][2] = 360;
        rent[29][3] = 850;
        rent[29][4] = 1025;
        rent[29][5] = 1200;
        rent[30][0] = 0;        // go to jail
        rent[30][1] = 0;
        rent[30][2] = 0;
        rent[30][3] = 0;
        rent[30][4] = 0;
        rent[30][5] = 0;
        rent[31][0] = 26;
        rent[31][1] = 130;
        rent[31][2] = 390;
        rent[31][3] = 900;
        rent[31][4] = 1100;
        rent[31][5] = 1275;
        rent[32][0] = 26;    //north carolina avenue
        rent[32][1] = 130;
        rent[32][2] = 390;
        rent[32][3] = 900;
        rent[32][4] = 1100;
        rent[32][5] = 1275;
        rent[33][0] = 0;        //community chest
        rent[33][1] = 0;
        rent[33][2] = 0;
        rent[33][3] = 0;
        rent[33][4] = 0;
        rent[33][5] = 0;
        rent[34][0] = 28;        //pensylvania ave
        rent[34][1] = 150;
        rent[34][2] = 450;
        rent[34][3] = 1000;
        rent[34][4] = 1200;
        rent[34][5] = 1400;
        rent[35][0] = 25;        //RR
        rent[35][1] = 50;
        rent[35][2] = 100;
        rent[35][3] = 200;
        rent[35][4] = 200;
        rent[35][5] = 200;
        rent[36][0] = 0;        //chance
        rent[36][1] = 0;
        rent[36][2] = 0;
        rent[36][3] = 0;
        rent[36][4] = 0;
        rent[36][5] = 0;
        rent[37][0] = 35;        //park place
        rent[37][1] = 175;
        rent[37][2] = 500;
        rent[37][3] = 1100;
        rent[37][4] = 1300;
        rent[37][5] = 1500;
        rent[38][0] = 75;        //luxury tax
        rent[38][1] = 75;
        rent[38][2] = 75;
        rent[38][3] = 75;
        rent[38][4] = 75;
        rent[38][5] = 75;
        rent[39][0] = 50;        // boardwalk
        rent[39][1] = 200;
        rent[39][2] = 600;
        rent[39][3] = 1400;
        rent[39][4] = 1700;
        rent[39][5] = 2000;
        
        
        
        var colorgroups = [null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"] //null if not property
        for(var i = 0; i < 40; i++)
        {
            var newTile = Tile(spaces[i], values[i], rent[i]); //more interitance
            this.tiles.push(newTile);
        }
        this.pieces = new Array();
        var Racecar = new Piece("racecar", 0);
        var Tophat = new Piece("tophat", 1);
        this.pieces.push(Racecar);
        this.pieces.push(Tophat);
    }
}

var Owner
{
    function Owner(color, ID)
    {
        this.id = ID;
        this.Properties = new Array(); //empty to start, no owned properties by default
        this.money = 1500;
        this.Assets = new Array(); //empty to start, obviously
        this.Color = color;
    }
}

var Tile
{
    function Tile(title, value, rents, mortgage, colorGroup)
    {
        this.title = title;
        if(colorGroup != null)
        {
            this.property = Property(value, rents, mortgage, colorGroup);
        }
        else
        {
            this.property = null;
            if(title == "Go")
            {
                //this.action=
            }
            else if(title == "Chance")
            {
                //random number
                //this.action = Chance[rand]; //not in constructor
            }
            //for all possible action tiles
        }
    }
}

var Property
{
    function Property(value, rents, mortgage, colorGroup)
    {
        this.mortgage = mortgage;
        this.isMortgaged = false;
        this.value = value;
        this.buildings = 0;
        this.rents = rents;
        this.colorGroup = colorGroup;
        this.player = null; //person that owns the property
    }
    function setOwn(cplayer)
    {
        this.player = cplayer;
    }
    function addBuilding()
    {
        this.buildings++;
    }
}

var Card
{
    function Card(type, text, m, value)
    {
        this.type = type;
        this.text = text;
        this.command = new Action(m, value);
    }
}

var Action
{
    function Action(m, value)
    {
        this.m = m;
        this.value = value;
    }
}

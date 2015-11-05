function main()
{
    var GameBoard = new Board();
    beginGameplay(GameBoard);
}

function beginGameplay(GameBoard)
{
    var turncounter = 0;
    var gameOver = false;
    while(!gameOver)
    {
        var currPlayer = GameBoard.pieces[turncounter]; //of type piece
        currPlayer.owner.Dice();
        if()//unowned...
        {
            //increment turn counter if not flagged
        }
        //game end condition
    }
}

class Piece
{
    constructor(Owner, type, color)
    {
        this.owner = Owner(color);
        this.type = type;
        this.position = Board.tiles[0]; //can we even do this??
        this.doubcount = 0;
        this.isJailed = false;
        this.location = 0;
    }
    function Dice()
    {
        //var die1 = document.getElementById("die1");
        //var die2 = document.getElementById("die2");
        var d1 = Math.floor(Math.random() * 6) + 1;
        var d2 = Math.floor(Math.random() * 6) + 1;
        var diceTotal = d1 + d2;
        if(d1 == d2)
        {
            doubcount = doubcount + 1;
            if(doubcount == 3)
            {
                console.log("Three sets of doubles, go to jail");
                location = 10;
                isJailed = true;
            }
            //status.innerHTML += " DOUBLES! You get a free turn!!";
            else
            {
                location = (location + diceTotal) % 40;
                //edit position somehow
                console.log("Move forward " + diceTotal);
                console.log("Doubles, go again!");
                //adjust turn counter (use a flag?)
            }
        }
        else
        {
            location = (location + diceTotal) % 40;
            //edit position somehow
            console.log("Move forward " + diceTotal);
            console.log("You are on " + Board.spaces[location] + " pay $" + Board.rent[location]);  //add houses rates
            doubcount = 0;
        }
        //return diceTotal;
    }
}

class Board()
{
    constructor()
    {
        this.tiles = new Array();
        var values = [0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400] //all values correspond with title index
        var spaces = ["Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"]
        var mortgages = [0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200]
        //matrix of rents
        var rent = []
        var colorgroups = [null, "brown", null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"] //null if not property
        for(var i = 0; i < 40; i++)
        {
            var newTile = Tile(spaces[i], values[i]); //more interitance
            tiles.push(newTile);
        }
        this.pieces = new Array();
        pieces.push("racecar");
        pieces.push("tophat");
    }
}

class Owner
{
    constructor(color)
    {
        this.Properties = new Array(); //empty to start, no owned properties by default
        this.money = 1500;
        this.Assets = new Array(); //empty to start, obviously
        this.Color = color;
    }
}

class Tile
{
    constructor(title, value, rents, mortgage, colorGroup)
    {
        this.title = title;
        if(colorGroup != null)
        {
            this.property = Property(value, rents, mortgage, colorGroup);
        }
        else
        {
            if(title == "Go")
            {
                this.action=
            }
            else if(title == "Chance")
            {
                //random number
                this.action = Chance[rand]; //not in constructor
            }
            //for all possible action tiles
        }
    }
}

class Property
{
    constructor(value, rents, mortgage, colorGroup)
    {
        this.mortgage = mortgage;
        this.isMortgaged = false;
        this.value = value;
        this.buildings = 0;
        this.rents = rents;
        this.colorGroup = colorGroup;
        this.player = null;
    }
}

class Card
{
    constructor(type, text, m, value)
    {
        this.type = type;
        this.text = text;
        this.command = new Action(m, value);
    }
}

class Action
{
    constructor(m, value)
    {
        this.m = m;
        this.value = value;
    }
}

function Piece(Owner, type, color)
{
    this.owner = Owner(color);
    this.type = type;
    this.position = Board.tiles[0];
}

function Board()
{
    this.tiles = new Array();
<<<<<<< HEAD
    var spaces = ["Go", "Meditteranean Avenue", ""] //add all tiles
    var values = [0, 60, 0, 60, 0, 200, 100, 0, 100, 120, 0, 140, 150, 140, 160, 200, 180, 0, 180, 200, 0, 220, 0, 220, 240, 200, 260, 260, 150, 280, 0, 300, 300, 0, 320, 200, 0, 350, 0, 400] //all values correspond with title index
    var mortgages = [] //mortgage values here
=======
    var spaces = ["Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B&O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"] //add all tiles
    var values = [0, 60, 0, 60] //all values correspond with title index
    var mortgages = [0, 30, 0, 30, 0, 100, 50, 0, 50, 60, 0, 70, 75, 70, 80, 100, 90, 0, 90, 100, 0, 110, 0, 110, 120, 100, 130, 130, 75, 140, 0, 150, 0, 150, 160, 100, 0, 175, 0, 200] //mortgage values here
>>>>>>> origin/master
    //matrix of rents
    var colorgroups = [null, "brown",null, "brown", null, "RR", "powder", null, "powder", "powder", null, "pink", "UTIL", "pink", "pink", "RR", "orange", null,"orange", "orange", null, "red", null, "red", "red", "RR", "yellow", "yellow", "UTIL", "yellow", null, "green", "green", null, "green", "RR", null, "blue", null, "blue"] //(null if not property)
    for(var i = 0; i < 40; i++)
    {
        var newTile = Tile(spaces[i], values[i]); //more interitance
        tiles.push(newTile);
    }
    this.pieces = new Array();
    pieces.push("racecar");
    pieces.push("tophat");
}

function Owner(color)
{
    this.Proerties = new Array(); //empty to start, no owned properties by default
    this.money = 1500;
    this.Assets = new Array(); //empty to start, obviously
    this.Color = color;
}

function Tile(title, value, rents, mortgage, colorGroup)
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

function Property(value, rents, mortgage, colorGroup)
{
    this.mortgage = mortgage;
    this.isMortgaged = false;
    this.value = value;
    this.buildings = 0;
    this.rents = rents;
    this.colorGroup = colorGroup;
    this.player = null;
}

function Card(type, text, m, value)
{
    this.type = type;
    this.text = text;
    this.command = new Action(m, value);
}

function Action(m, value)
{
    this.m = m;
    this.value = value;
}

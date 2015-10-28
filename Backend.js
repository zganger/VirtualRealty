function Piece(Owner, type, color)
{
    this.owner = Owner(color);
    this.type = type;
    this.position = Board.tiles[0];
}

function Board()
{
    this.tiles = new Array();
    var spaces = ["Go", "Meditteranean Avenue", ""] //add all tiles
    var values = [0, 60, 0, 60] //all values correspond with title index
    var mortgages = [] //mortgage values here
    //matrix of rents
    //colorgroups (null if not property)
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
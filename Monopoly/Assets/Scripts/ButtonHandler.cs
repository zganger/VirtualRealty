using UnityEngine;

public class ButtonHandler : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public void RollClicked()
	{
		GamePlay.GameBoard.state = 1;
	}

	public void PropertyYesClicked ()
	{
		GamePlay.GameBoard.state = 4;
	}

	public void PropertyNoClicked ()
	{
		GamePlay.GameBoard.state = 0;
	}

	public void RentOkClicked ()
	{
		GamePlay.GameBoard.state = 2;
	}

    public void BuyBuildingClicked()
    {
        GamePlay.GameBoard.backupState = GamePlay.GameBoard.state;
        GamePlay.GameBoard.state = 6;
    }
    
    public void CancelClicked()
    {
        GamePlay.GameBoard.state = GamePlay.GameBoard.backupState;
    }

	public void MediterraneanBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 1;
	}

	public void BalticBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 3;
    }

	public void OrientalBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 6;
    }

	public void VermontBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 8;
    }

	public void ConnecticutBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 9;
    }

	public void StCharlesBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 11;
    }

	public void StatesBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 13;
    }

	public void VirginiaBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 14;
    }

	public void StJamesBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 16;
    }

	public void TennesseeBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 18;
    }

	public void NewYorkBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 19;
    }

	public void KentuckyBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 21;
    }

	public void IndianaBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 23;
    }

	public void IllinoisBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 24;
    }

	public void AtlanticBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 26;
    }

	public void VentnorBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 27;
    }

	public void MarvinGardensBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 29;
    }

	public void PacificBuilding()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 31;
    }

	public void NorthCarolinaBuilding()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 32;
    }

	public void PennsylvaniaBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 34;
    }

	public void ParkPlaceBuilding()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 37;
    }

	public void BoardwalkBuilding ()
	{
        GamePlay.GameBoard.state = 7;
        GamePlay.GameBoard.toAddBuilding = 39;
    }
}

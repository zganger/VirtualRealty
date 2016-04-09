using UnityEngine;

public class ButtonHandler : MonoBehaviour {

    void Start () {

    }

	// Update is called once per frame
	void Update () {
	    //change scenes

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
}

using UnityEngine;

public class ButtonHandler : MonoBehaviour {
	public void RollClicked()
	{
		GamePlay.GameBoard.State = 1;
	}

	public void PropertyYesClicked ()
	{
		GamePlay.GameBoard.State = 4;
	}

	public void PropertyNoClicked ()
	{
		GamePlay.GameBoard.State = 0;
	}

	public void RentOkClicked ()
	{
		GamePlay.GameBoard.State = 2;
	}

    public void BuyBuildingClicked()
    {
        GamePlay.GameBoard.BackupState = GamePlay.GameBoard.State;
        GamePlay.GameBoard.State = 6;
    }
    
    public void CancelClicked()
    {
        GamePlay.GameBoard.State = GamePlay.GameBoard.BackupState;
    }
}

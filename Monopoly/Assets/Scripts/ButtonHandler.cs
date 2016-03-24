using UnityEngine;
using System.Collections;

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
}

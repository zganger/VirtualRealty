using UnityEngine;
using System.Collections;

public class UnityPieceImage : MonoBehaviour {

	bool movementFinished;
	public int Dest;

	public bool isFinished ()
	{
		return movementFinished;
	}

	// Use this for initialization
	void Start () 
	{

	}
/*
	void Update ()
	{
		movementFinished = false;
		while (!movementFinished) {
			if ((transform.position.x + 200 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) || transform.position.x - 200 <= GamePlay.GameBoard.getUnityCoords (Dest, 0)) && (transform.position.y + 200 >= GamePlay.GameBoard.getUnityCoords (Dest, 1) || transform.position.y - 200 <= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
				movementFinished = true;
			} else if (transform.position.x > 240 && transform.position.y < 41) { //will skip the tiniest bit
				transform.position = new Vector3 (transform.position.x - 60 * Time.deltaTime, 40, 0);
			} else if (transform.position.x <= 240 && transform.position.y <= 350) {
				transform.position = new Vector3 (240, transform.position.y + 60 * Time.deltaTime, 0);	//this is the left
			} else if (transform.position.x <= 550 && transform.position.y >= 350) {
				transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, 350, 0);	//this is top
			} else {
				transform.position = new Vector3 (550, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
			}
		}
	}
*/

	void Update ()
	{
		movementFinished = true;
		//check if coordinates are in range of the specified location
		if (!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1)))
		{ movementFinished = false; } 
		else if (!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1)))
		{ movementFinished = false; } 
		else if(!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1)))
		{ movementFinished = false; } 
		else if(!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1)))
		{ movementFinished = false; }
		while (!movementFinished)
		{
			if (transform.position.x >= 150 && transform.position.y <= -150) { //THESE NEED A BUFFER AND IT SHOULD WORK!
				moveLeft();
			} else if (transform.position.x <= -150 && transform.position.y <= -150) {
				moveUp();
			} else if (transform.position.x <= -150 && transform.position.y >= 150) {
				moveRight();
			} else if(transform.position.x >= 150 && transform.position.y >= 150){
				moveDown();
			}
			movementFinished=true;
		}
	}
	public void moveLeft ()
	{
		Debug.Log("moving Left");
		transform.position = new Vector3 (transform.position.x - 60 * Time.deltaTime, -150, 0);	//this is bottom
	}
	public void moveUp ()
	{
		Debug.Log("moving Up");
		transform.position = new Vector3 (-150, transform.position.y + 60 * Time.deltaTime, 0);	//this is the left
	}
	public void moveRight ()
	{
		Debug.Log("moving Right");
		transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, 150, 0);
	}
	public void moveDown ()
	{
		Debug.Log("moving Down");
		transform.position = new Vector3 (150, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
	}
	public void MoveTo (int inDest)
	{
		Dest = inDest;
	}
}

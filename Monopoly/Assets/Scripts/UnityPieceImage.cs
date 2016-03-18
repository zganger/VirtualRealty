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
	float offX = 347.2f;	//offset
	float offY = 182f;		//offset
	public float xLenient, yLenient;
	void Update ()
	{
		movementFinished = true;
		//check if coordinates are in range of the specified location
		if (!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
//			Debug.Log("hello1");
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if (!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1))) { 	
			Debug.Log("hello2");
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if(!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
//			Debug.Log("hello3");
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if(!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
//			Debug.Log("hello4");
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		}
		while (!movementFinished)
		{
//			Debug.Log(xLenient + "," + yLenient);
			if (xLenient >= -148 && yLenient <= -148) { //THESE NEED A BUFFER AND IT SHOULD WORK!
				moveLeft();
			} else if (xLenient <= -148 && yLenient <= 148) {
				moveUp();
			} else if (xLenient <= 148  && yLenient >= 148 ) {//(xLenient <= -150  &&)
				moveRight();
			} else {
				moveDown();
			}
			movementFinished=true;
			Debug.Log("STOP MOVING");
		}
	}

	public void moveLeft ()
	{
//		Debug.Log("moving Left");
		transform.position = new Vector3 (transform.position.x - 60 * Time.deltaTime, transform.position.y, 0);	//this is bottom
	}
	public void moveUp ()
	{
//		Debug.Log("moving Up");
		transform.position = new Vector3 (transform.position.x, transform.position.y + 60 * Time.deltaTime, 0);	//this is the left
	}
	public void moveRight ()
	{
//		Debug.Log("moving Right");
		transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, transform.position.y, 0);
	}
	public void moveDown ()
	{
//		Debug.Log("moving Down");
		transform.position = new Vector3 (transform.position.x, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
	}
	public void MoveTo (int inDest)
	{
		Dest = inDest;
	}
}

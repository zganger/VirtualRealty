using UnityEngine;

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

	float offX = 513.2f;	//offset
	float offY = 398f;		//offset
	public float xLenient, yLenient;
	void Update ()
	{
		//check if coordinates are in range of the specified location
		if (!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if (!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 1))) { 	
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if(!(transform.position.x + 5 >= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		} else if(!(transform.position.x - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y - 5 <= GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
			xLenient = transform.position.x-offX;	//ACCOUNT FOR DIFFERENT ORIGINS
			yLenient = transform.position.y-offY;	//ACCOUNT FOR DIFFERENT ORIGINS
			movementFinished = false;
		}
		while (!movementFinished)
		{
//			Debug.Log("Dest: " + GamePlay.GameBoard.getUnityCoords (Dest, 0) + ", " + GamePlay.GameBoard.getUnityCoords (Dest, 1));
//			Debug.Log(xLenient + "," + yLenient);
			if (xLenient >= -148 && yLenient <= -148) {
//				moveLeft();
			} else if (xLenient <= -148 && yLenient <= 148) {
//				moveUp();
			} else if (xLenient <= 148  && yLenient >= 148 ) {//(xLenient <= -150  &&)
//				moveRight();
			} else {
//				moveDown();
			}
			movementFinished=true;
		}
	}
/*
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
*/
	public void MoveTo (int inDest)
	{
		Dest = inDest;
		transform.position = new Vector3 (GamePlay.GameBoard.getUnityCoords (Dest, 0) + offX, GamePlay.GameBoard.getUnityCoords (Dest, 1) + offY, 0);	//this is the right
	}
}

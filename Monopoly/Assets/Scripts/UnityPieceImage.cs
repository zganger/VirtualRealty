using UnityEngine;
using System.Collections;

public class UnityPieceImage : MonoBehaviour {

	bool movementFinished;

	// Use this for initialization
	void Start () 
	{

	}

	public int Dest;

	// Update is called once per frame
	void Update ()
	{
		movementFinished = true;
		if (!(transform.position.x == GamePlay.GameBoard.getUnityCoords (Dest, 0) && transform.position.y != GamePlay.GameBoard.getUnityCoords (Dest, 1))) {
			movementFinished = false;
		}
		while (movementFinished== false && (transform.position.x != GamePlay.GameBoard.getUnityCoords(Dest, 0) || transform.position.y != GamePlay.GameBoard.getUnityCoords(Dest, 1))) {
			if (transform.position.x > 240 && transform.position.y < 41) { //will skip the tiniest bit
				transform.position = new Vector3 (transform.position.x - 60 * Time.deltaTime, 40, 0);
			} else if (transform.position.x <= 240 && transform.position.y <= 350) {
				transform.position = new Vector3 (240, transform.position.y + 60 * Time.deltaTime, 0);	//this is the left
			} else if (transform.position.x <= 550 && transform.position.y >= 350) {
				transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, 350, 0);	//this is top
			} else {
				transform.position = new Vector3 (550, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
			}
			movementFinished=true;
		}
	}

	public void MoveTo (int inDest)
	{
		Dest = inDest;
	}
}

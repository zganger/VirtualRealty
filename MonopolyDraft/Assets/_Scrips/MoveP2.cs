using UnityEngine;
using System.Collections;
using Backend;

public class MoveP2 : MonoBehaviour {

	// Use this for initialization
	void Start () 
	{

	}

	public int Dest, Origin, diceTotal;

	// Update is called once per frame
	void Update ()
	{

	}

	public void MoveTo (int inDest, Board gameBoard)
	{
//		Dest = inDest;
//		Origin = inOrigin;
//		diceTotal = inDice;
		bool isFinished = false;
		Debug.Log("MovedTo");
		while (isFinished==false && (transform.position.x != gameBoard.Unitycoordinates [inDest, 0] || transform.position.x != gameBoard.Unitycoordinates [inDest, 1])) {
			Debug.Log ("enter while loop");
			if (transform.position.x > 240 && transform.position.y < 42) { //will skip the tiniest bit
				transform.position = new Vector3 (transform.position.x - 60,40,0);// * Time.deltaTime, 40, 0);
			} else if (transform.position.x <= 240 && transform.position.y <= 350) {
				transform.position = new Vector3 (240, transform.position.y + 60,0 );//* Time.deltaTime, 0);	//this is the left
			} else if (transform.position.x <= 550 && transform.position.y >= 350) {
				transform.position = new Vector3 (transform.position.x + 60,350,0 );//* Time.deltaTime, 350, 0);	//this is top
			} else {
				transform.position = new Vector3 (550, transform.position.y - 60,0);// * Time.deltaTime, 0);	//this is the right
			}
			isFinished = true;
			Debug.Log(transform.position);
		}
//		while (transform.position.x != inDest)
//			{
//				transform.position = new Vector3(transform.position.x - 60 * Time.deltaTime, 40, 0);
//			}
	}
}

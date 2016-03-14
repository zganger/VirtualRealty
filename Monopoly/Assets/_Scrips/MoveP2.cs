﻿using UnityEngine;
using System.Collections;

public class MoveP2 : MonoBehaviour {

	// Use this for initialization
	void Start () 
	{

	}

	public int Dest, Origin, diceTotal;

	// Update is called once per frame
	void Update ()
	{
		bool isFinished = false;
		while (isFinished==false && (transform.position.x != GamePlay.GameBoard.Unitycoordinates [Dest, 0])) { //|| transform.position.x != gameBoard.Unitycoordinates [inDest, 1])) {
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
		}
	}

	public void MoveTo (int inDest, Board gameBoard)
	{
		Dest = inDest;
	}
}

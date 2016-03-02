using UnityEngine;
using System.Collections;

namespace ourUnityThings
{
	public class MoveP1 : MonoBehaviour
	{
//		int Dest, Origin, diceTotal;
		// Use this for initialization
		void Start ()
		{
//			MoveTo(Origin, Dest, diceTotal);
		}
		void Update ()
		{
//			MoveTo(Origin, Dest, diceTotal);
		}
	
		// Update is called once per frame
		void motionP1 () 	//motionP1
		{
			if (transform.position.x > 260 && transform.position.y < 22)
			{
				transform.position = new Vector3(transform.position.x - 60 * Time.deltaTime, 20 , 0);	//20 units per second
			}
			else if (transform.position.x <= 260 && transform.position.y <= 330)
			{
				transform.position = new Vector3(260, transform.position.y + 60 * Time.deltaTime, 0);	//20 units per second
			}
			else if (transform.position.x <= 570 && transform.position.y >= 330) 
			{
				transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, 330, 0);	//this is top
			}
			else 
			{
				transform.position = new Vector3 (570, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
			}
		}

		void motionP2 ()	//p2
		{
			if (transform.position.x > 240 && transform.position.y < 42) //will skip the tiniest bit
			{
				transform.position = new Vector3 (transform.position.x - 60 * Time.deltaTime, 40, 0);
			}
			else if (transform.position.x <= 240 && transform.position.y <= 350) 
			{
				transform.position = new Vector3 (240, transform.position.y + 60 * Time.deltaTime, 0);	//this is the left
			}
			else if (transform.position.x <= 550 && transform.position.y >= 350) 
			{
				transform.position = new Vector3 (transform.position.x + 60 * Time.deltaTime, 350, 0);	//this is top
			}
			else 
			{
				transform.position = new Vector3 (550, transform.position.y - 60 * Time.deltaTime, 0);	//this is the right
			}
		}

		public void MoveTo (int inOrigin, int inDest, int inDice)
		{
			while (transform.position.x != inDest)
			{
				transform.position = new Vector3(transform.position.x - 60 * Time.deltaTime, 40, 0);
			}
		}
	}
}

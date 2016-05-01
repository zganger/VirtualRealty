using UnityEngine;

public class UnityPieceImage : MonoBehaviour {

	bool movementFinished = true;
	public int Dest;
    private Vector3 targetposition;
    float Speed = 1;

    // Use this for initialization
    void Start () 
	{

	}

    public bool isFinished()
    {
        return movementFinished;
    }

	public float xLenient, yLenient;
	void Update ()
	{
        if (movementFinished == false)
        {
            if (((Dest / 10) % 2 == 1))
            {
                if (transform.position.z != targetposition.z)
                {
                    Vector3 moveZ = targetposition;
                    moveZ.x = transform.position.x;
                    transform.position = Vector3.MoveTowards(transform.position, moveZ, Speed * Time.deltaTime);
                }
                else if (transform.position.x != targetposition.x)
                {
                    Vector3 moveX = targetposition;
                    moveX.z = transform.position.z;
                    transform.position = Vector3.MoveTowards(transform.position, moveX, Speed * Time.deltaTime);
                }
                else
                {
                    movementFinished = true;
                }
            }
            else
            {
                if (transform.position.x != targetposition.x)
                {
                    Vector3 moveX = targetposition;
                    moveX.z = transform.position.z;
                    transform.position = Vector3.MoveTowards(transform.position, moveX, Speed * Time.deltaTime);
                }
                else if (transform.position.z != targetposition.z)
                {
                    Vector3 moveZ = targetposition;
                    moveZ.x = transform.position.x;
                    transform.position = Vector3.MoveTowards(transform.position, moveZ, Speed * Time.deltaTime);
                }
                else
                {
                    movementFinished = true;
                }
            }
        }
    }

	public void MoveTo (int inDest, int ID)
	{
		Dest = inDest;
        movementFinished = false;
        if (ID == 1)
        {
            if (Dest == 0)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (0 < Dest && Dest < 10)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (10 < Dest && Dest < 20)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0) - 0.45f, 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1) - 0.35f);	//this is the right
            }
            if (20 < Dest && Dest < 30)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0) - 1.4f, 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (30 < Dest && Dest < 39)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0) - 0.35f, 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1) - 0.35f);	//this is the right
            }
            
        }
        else
        {
            if (Dest == 0)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (0 < Dest && Dest < 10)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (10 < Dest && Dest < 20)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1) + 0.15f);	//this is the right
            }
            if (20 < Dest && Dest < 30)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
            }
            if (30 < Dest && Dest < 39)
            {
                targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1) + 0.15f);	//this is the right
            }
        }
        Speed = 3f;
	}
}

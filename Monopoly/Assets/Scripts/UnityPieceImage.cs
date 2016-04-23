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
            targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0) - 0.723f, 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
        }
        else
        {
            targetposition = new Vector3(GamePlay.GameBoard.getUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.getUnityCoords(Dest, 1));	//this is the right
        }
        Speed = 3f;
	}
}

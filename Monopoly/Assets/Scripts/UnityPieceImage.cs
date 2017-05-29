using UnityEngine;

public class UnityPieceImage : MonoBehaviour {
    private bool _movementFinished = true;
	public int Dest;
    private Vector3 _targetposition;
    private float _speed = 1;

    // Use this for initialization

    public bool IsFinished()
    {
        return _movementFinished;
    }

	public float XLenient, YLenient;

    private void Update ()
	{
        if (_movementFinished == false)
        {
            if (Dest / 10 % 2 == 1)
            {
                if (transform.position.z != _targetposition.z)
                {
                    var moveZ = _targetposition;
                    moveZ.x = transform.position.x;
                    transform.position = Vector3.MoveTowards(transform.position, moveZ, _speed * Time.deltaTime);
                }
                else if (transform.position.x != _targetposition.x)
                {
                    var moveX = _targetposition;
                    moveX.z = transform.position.z;
                    transform.position = Vector3.MoveTowards(transform.position, moveX, _speed * Time.deltaTime);
                }
                else
                {
                    _movementFinished = true;
                }
            }
            else
            {
                if (transform.position.x != _targetposition.x)
                {
                    var moveX = _targetposition;
                    moveX.z = transform.position.z;
                    transform.position = Vector3.MoveTowards(transform.position, moveX, _speed * Time.deltaTime);
                }
                else if (transform.position.z != _targetposition.z)
                {
                    var moveZ = _targetposition;
                    moveZ.x = transform.position.x;
                    transform.position = Vector3.MoveTowards(transform.position, moveZ, _speed * Time.deltaTime);
                }
                else
                {
                    _movementFinished = true;
                }
            }
        }
    }

	public void MoveTo (int inDest, int id)
	{
		Dest = inDest;
        _movementFinished = false;
        if (id == 1)
        {
            if (Dest == 0)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (0 < Dest && Dest < 10)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (10 < Dest && Dest < 20)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0) - 0.45f, 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1) - 0.35f);	//this is the right
            if (20 < Dest && Dest < 30)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0) - 1.4f, 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (30 < Dest && Dest < 39)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0) - 0.35f, 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1) - 0.35f);	//this is the right
            
        }
        else
        {
            if (Dest == 0)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (0 < Dest && Dest < 10)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (10 < Dest && Dest < 20)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1) + 0.15f);	//this is the right
            if (20 < Dest && Dest < 30)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1));	//this is the right
            if (30 < Dest && Dest < 39)
                _targetposition = new Vector3(GamePlay.GameBoard.GetUnityCoords(Dest, 0), 1.135f, GamePlay.GameBoard.GetUnityCoords(Dest, 1) + 0.15f);	//this is the right
        }
        _speed = 3f;
	}
}

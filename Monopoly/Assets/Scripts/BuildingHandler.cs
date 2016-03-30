using UnityEngine;
using System.Collections;

public class BuildingHandler : MonoBehaviour {

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {

	}

    public void MoveDown()
    {
        transform.position = new Vector3(transform.position.x, transform.position.y - 3, transform.position.z);  //this is the right
    }

    public void MoveUp()
    {
        transform.position = new Vector3(transform.position.x, transform.position.y + 3, transform.position.z);  //this is the right
    }
}

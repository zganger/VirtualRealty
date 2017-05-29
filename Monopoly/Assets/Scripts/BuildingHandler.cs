using UnityEngine;

public class BuildingHandler : MonoBehaviour {

    public bool IsUp;

    public void MoveDown()
    {
        transform.position = new Vector3(transform.position.x, transform.position.y - 3, transform.position.z);  //this is the right
        IsUp = false;
    }

    public void MoveUp()
    {
        transform.position = new Vector3(transform.position.x, transform.position.y + 3, transform.position.z);  //this is the right
        IsUp = true;
    }
}

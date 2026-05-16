using System;
using Engine;

class ControllerConnectedScript : MonoBehaviour
{
	Renderer render;
	float lifeTime = 3.0f;


    public override void Awake()
	{
        render = gameobject.GetComponent<Renderer>();
    }

	public override void Start()
    {
	}

    public override void Update()
    {
		lifeTime -= Time.deltaTime;
        if(lifeTime <= 0.0f)
            gameobject.Destroy();
    }
}
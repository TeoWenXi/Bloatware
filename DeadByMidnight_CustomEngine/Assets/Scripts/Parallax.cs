using System;
using Engine;

class Parallax : MonoBehaviour
{
	Renderer render;
	float moveTime = 0.0f;
    //Serialized values
    public float ParallaxXRate;
    public float ParallaxYRate;
    public float TrafficSpeed;
    public float AlphaMultiplier;
    public Vector4 TexST;



    public override void Awake()
	{
        render = gameobject.GetComponent<Renderer>();

        render.SetUniform("_Time", moveTime);
        render.SetUniform("_ParallaxXRate", ParallaxXRate);
        render.SetUniform("_ParallaxYRate", ParallaxYRate);
        render.SetUniform("_XCoord", Camera.position.x);
        render.SetUniform("_YCoord", Camera.position.y);
        render.SetUniform("_TrafficSpeed", TrafficSpeed);
        render.SetUniform("_AlphaMultiplier", AlphaMultiplier);
        render.SetUniform("_Tex_ST", TexST);
    }

	public override void Start()
    {
	}

    public override void Update()
    {
		moveTime += Time.deltaTime;
        render.SetUniform("_Time", moveTime);
        render.SetUniform("_ParallaxXRate", ParallaxXRate);
        render.SetUniform("_ParallaxYRate", ParallaxYRate);
        render.SetUniform("_XCoord", Camera.position.x);
        render.SetUniform("_YCoord", Camera.position.y);
        render.SetUniform("_TrafficSpeed", TrafficSpeed);
        render.SetUniform("_AlphaMultiplier", AlphaMultiplier);
        render.SetUniform("_Tex_ST", TexST);
    }
}
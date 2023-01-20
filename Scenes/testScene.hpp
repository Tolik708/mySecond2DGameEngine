namespace scene
{
	void initScene()
	{
		mesh cube = mesh(box, vec2(10, 10), Color(true));
		GameObject test1 = AddObj(vec2(250, 250), &cube);
	}
}
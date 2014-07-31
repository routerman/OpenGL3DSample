/*
class shape{
public:
	virtual bool detect(shape& sh);
	virtual bool is_collision(box& b);
	virtual bool is_collision(sphere& s);
};

class sphere : public shape{
	F3 position;
	float radius;

public:
	virtual bool detect(shape& sh){
		return sh.is_collision(*this);
	}

	virtual bool is_collision(box& b){
		return false;
	}

	virtual bool is_collision(sphere& s){
		return false;
	}
};

class box : public shape{
	F3 position;
	F3 size;

public:
	virtual bool detect(shape& sh){
		return sh.is_collision(*this);
	}

	virtual bool is_collision(box& b){
		return false;
	}

	virtual bool is_collision(sphere& s){
		return false;
	}
};
*/

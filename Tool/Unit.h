#pragma once
class CUnit
{
public:
	CUnit();
	virtual ~CUnit();


public:
	void		Initialize(void);
	void		Update();
	void		Render();
	void		Release();
};


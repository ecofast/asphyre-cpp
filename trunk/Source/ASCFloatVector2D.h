/***************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCFloatVector2D.h
 *  Content: Definitions and functions working with 2D float vectors
 *  Date   : 2013/05/31
 *  Desc   : Types, functions and classes to facilitate working with 2D float vectors
 *
 ***************************************************************************************/

#pragma once

struct CASCFloatVector2D
{
	float X, Y;

	CASCFloatVector2D()
	{
		X = Y = 0;
	}

	CASCFloatVector2D(const CASCFloatVector2D &vec)
	{
		X = vec.X;
		Y = vec.Y;
	}

	CASCFloatVector2D(float fX, float fY)
	{
		X = fX;
		Y = fY;
	}

	CASCFloatVector2D operator+(const CASCFloatVector2D& vec) const
	{
		return CASCFloatVector2D(X + vec.X, Y + vec.Y);
	}

	CASCFloatVector2D operator-(const CASCFloatVector2D& vec) const
	{
		return CASCFloatVector2D(X - vec.X, Y - vec.Y);
	}

	bool Equals(const CASCFloatVector2D& vec)
	{
		return ((X == vec.X) && (Y == vec.Y));
	}
};
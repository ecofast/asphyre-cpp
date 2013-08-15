/***************************************************************************************
 *
 *  Copyright (C) Yuriy Kotsarenko / HuGuangyao.  All Rights Reserved.
 *
 *  File   : ASCIntVector2D.h
 *  Content: Definitions and functions working with 2D integer vectors
 *  Date   : 2013/05/31
 *  Desc   : Types, functions and classes to facilitate working with 2D integer vectors
 *
 ***************************************************************************************/

#pragma once

struct CASCIntVector2D
{
	int X, Y;

	CASCIntVector2D()
	{
		X = Y = 0;
	}

	CASCIntVector2D(const CASCIntVector2D &vec)
	{
		X = vec.X;
		Y = vec.Y;
	}

	CASCIntVector2D(int nX, int nY)
	{
		X = nX;
		Y = nY;
	}

	CASCIntVector2D operator+(const CASCIntVector2D& vec) const
	{
		return CASCIntVector2D(X + vec.X, Y + vec.Y);
	}

	CASCIntVector2D operator-(const CASCIntVector2D& vec) const
	{
		return CASCIntVector2D(X - vec.X, Y - vec.Y);
	}

	bool Equals(const CASCIntVector2D& vec)
	{
		return ((X == vec.X) && (Y == vec.Y));
	}
};
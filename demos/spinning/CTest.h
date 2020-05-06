#pragma once
#include <PQP.h>
#include "model.h"
#include "glm/glm.hpp"
class CMyTest
{
public:
    PQP_Model bunny, torus;
    Model bunny_to_draw, torus_to_draw;
    CMyTest();
    void test() {

    }
	void init() {

        // initialize the bunny

        bunny_to_draw.load("bunny.tris");
        bunny.BeginModel();
        for (int i = 0; i < bunny_to_draw.ntris; i++)
        {
            bunny.AddTri(&bunny_to_draw.tri[i].p[0][0],
                &bunny_to_draw.tri[i].p[1][0],
                &bunny_to_draw.tri[i].p[2][0], i);
        }
        bunny.EndModel();

        // initialize the torus

        torus_to_draw.load("torus2.tris");


        torus.BeginModel();
        for (int i = 0; i < torus_to_draw.ntris; i++)
        {
            torus.AddTri(&torus_to_draw.tri[i].p[0][0],
                &torus_to_draw.tri[i].p[1][0],
                &torus_to_draw.tri[i].p[2][0], i);
        }
        torus.EndModel();

	}
};


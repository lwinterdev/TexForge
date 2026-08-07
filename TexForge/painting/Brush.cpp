#include "Brush.h"

#include "PaintTexture.h"

#include <cmath>



void Brush::Paint(
    PaintTexture& texture,
    int centerX,
    int centerY
)
{

    int startX =
        centerX - radius;


    int endX =
        centerX + radius;


    int startY =
        centerY - radius;


    int endY =
        centerY + radius;



    for (
        int y = startY;
        y <= endY;
        y++
        )
    {

        for (
            int x = startX;
            x <= endX;
            x++
            )
        {

            float dx =
                x - centerX;


            float dy =
                y - centerY;



            float distance =
                sqrt(
                    dx * dx +
                    dy * dy
                );



            // outside brush circle

            if (distance > radius)
                continue;



            /*
                Soft brush falloff

                center:
                    strength = 1

                edge:
                    strength = 0
            */

            float strength =
                1.0f -
                distance / radius;



            strength *= opacity;



            glm::vec4 paintColor =
                color;


            paintColor.a =
                strength;



            texture.SetPixel(
                x,
                y,
                paintColor
            );

        }

    }

}
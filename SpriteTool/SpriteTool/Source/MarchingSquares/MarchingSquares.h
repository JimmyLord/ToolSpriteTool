// Licensed under the creative commons Atrribution 3.0 license
// You are free to share, copy, distribute, and transmit this work
// You are free to alter this work
// If you use this work, please attribute it somewhere in the supporting
// documentation of your work. (A mention in a readme or credits, for example.)
// Please leave a comment or email me if you use this work, so I am compelled
// to produce more of it's kind.
// 2010 - Phillip Spiess

class MarchingSquares
{
private:
    // A simple enumeration to represent the direction we
    // just moved, and the direction we will next move.
    enum StepDirection
    {
        StepDir_None,
        StepDir_Up,
        StepDir_Left,
        StepDir_Down,
        StepDir_Right
    };

    // Some variables to make our function
    // calls a little smaller, probably shouldn't
    // expose these to the public though.

    const unsigned char* m_Pixels;
    unsigned int m_Width;
    unsigned int m_Height;
    
    std::list<ivec2>* m_Results;

    // The direction we previously stepped
    StepDirection previousStep;

    // Our next step direction:
    StepDirection nextStep;

    // Takes a texture and returns a list of pixels that
    // define the perimeter of the upper-left most
    // object in that texture, using alpha==0 to determine
    // the boundary.
public:
    void DoMarch(const unsigned char* pixels, unsigned int w, unsigned int h, std::list<ivec2>* output)
    {
        m_Pixels = pixels;
        m_Width = w;
        m_Height = h;

        m_Results = output;

        // Find the start points
        ivec2 perimeterStart = FindStartPoint();

        // Return the list of points
        return WalkPerimeter( perimeterStart.x, perimeterStart.y );
    }

    // Finds the first pixel in the perimeter of the image
    ivec2 FindStartPoint()
    {
        ivec2 ret;

        // Scan along the whole image
        for( unsigned int i=0; i<m_Width*m_Height; i++ )
        {
            // If the pixel is not entirely transparent
            // we've found a start point
            if( m_Pixels[i*4 + 3] > 0 )
            {
                ret.x = i % m_Width;
                ret.y = i / m_Width;
                return ret;
            }
        }

        // If we get here
        // we've scanned the whole image and found nothing.
        ret.x = 0;
        ret.y = 0;
        return ret;
    }

    // Performs the main while loop of the algorithm
    void WalkPerimeter(unsigned int startX, unsigned int startY)
    {
        // Do some sanity checking, so we aren't
        // walking outside the image
        if( startX < 0 )
            startX = 0;
        if( startX > m_Width )
            startX = m_Width;
        if( startY < 0 )
            startY = 0;
        if( startY > m_Height )
            startY = m_Height;

        // Set up our return list
        //List<Vector2> pointList = new List<Vector2>();

        // Our current x and y positions, initialized
        // to the init values passed in
        unsigned int x = startX;
        unsigned int y = startY;

        // The main while loop, continues stepping until 
        // we return to our initial points
        do
        {
            // Evaluate our state, and set up our next direction
            Step( x, y );

            // If our current point is within our image
            // add it to the list of points
            if( x >= 0 && x < m_Width && y >= 0 && y < m_Height )
            {
                ivec2 temp;
                temp.x = x;
                temp.y = y;
                m_Results->push_back( temp );
            }

            switch (nextStep)
            {
                case StepDir_Up: y--; break;
                case StepDir_Left: x--; break;
                case StepDir_Down: y++; break;
                case StepDir_Right: x++; break;
                default:
                    break;
            }
        } while( x != startX || y != startY );

        //return m_Results;
    }

    // Determines and sets the state of the 4 pixels that
    // represent our current state, and sets our current and
    // previous directions
    void Step(int x, int y)
    {
        // Scan our 4 pixel area
        bool upLeft = IsPixelSolid(x - 1, y - 1);
        bool upRight = IsPixelSolid(x, y - 1);
        bool downLeft = IsPixelSolid(x - 1, y);
        bool downRight = IsPixelSolid(x, y);

        // Store our previous step
        previousStep = nextStep;

        // Determine which state we are in
        int state = 0;

        if (upLeft)
            state |= 1;
        if (upRight)
            state |= 2;
        if (downLeft)
            state |= 4;
        if (downRight)
            state |= 8;

        // State now contains a number between 0 and 15
        // representing our state.
        // In binary, it looks like 0000-1111 (in binary)

        // An example. Let's say the top two pixels are filled,
        // and the bottom two are empty.
        // Stepping through the if statements above with a state 
        // of 0b0000 initially produces:
        // Upper Left == true ==>  0b0001
        // Upper Right == true ==> 0b0011
        // The others are false, so 0b0011 is our state 
        // (That's 3 in decimal.)

        // Looking at the chart above, we see that state
        // corresponds to a move right, so in our switch statement
        // below, we add a case for 3, and assign Right as the
        // direction of the next step. We repeat this process
        // for all 16 states.

        // So we can use a switch statement to determine our
        // next direction based on
        switch (state)
        {
            case 1: nextStep = StepDir_Up; break;
            case 2: nextStep = StepDir_Right; break;
            case 3: nextStep = StepDir_Right; break;
            case 4: nextStep = StepDir_Left; break;
            case 5: nextStep = StepDir_Up; break;
            case 6:
                if (previousStep == StepDir_Up)
                {
                    nextStep = StepDir_Left;
                }
                else
                {
                    nextStep = StepDir_Right;
                }
                break;
            case 7: nextStep = StepDir_Right; break;
            case 8: nextStep = StepDir_Down; break;
            case 9:
                if (previousStep == StepDir_Right)
                {
                    nextStep = StepDir_Up;
                }
                else
                {
                    nextStep = StepDir_Down;
                }
                break;
            case 10: nextStep = StepDir_Down; break;
            case 11: nextStep = StepDir_Down; break;
            case 12: nextStep = StepDir_Left; break;
            case 13: nextStep = StepDir_Up; break;
            case 14: nextStep = StepDir_Left; break;
            default:
                nextStep = StepDir_None;
                break;
        }
    }

    // Determines if a single pixel is solid (we test against
    // alpha values, you can write your own test if you want
    // to test for a different color.)
    bool IsPixelSolid(unsigned int x, unsigned int y)
    {
        // Make sure we don't pick a point outside our
        // image boundary!
        if (x < 0 || y < 0 ||
            x >= m_Width || y >= m_Height)
            return false;

        // Check the color value of the pixel
        // If it isn't 100% transparent, it is solid
        if( m_Pixels[(y*m_Width + x)*4 + 3] > 0 )
            return true;

        // Otherwise, it's not solid
        return false;
    }
};

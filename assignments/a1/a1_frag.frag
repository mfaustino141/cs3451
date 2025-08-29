#version 330 core

uniform vec2 iResolution;               /* screen resolution, value passed from CPU */
uniform float iTime;                    /* current time, value passed from CPU */
uniform int iFrame;                     /* current frame, value passed from CPU */
in vec2 fragCoord;                      /* fragment shader input: fragment coordinates, valued passed from vertex shader */
out vec4 fragColor;                     /* fragment shader output: fragment color, value passed to pixel processing for screen display */

const float M_PI = 3.1415926535;                        /* const value for PI */
const vec3 BG_COLOR = vec3(184, 243, 255) / 255.;       /* const value for background color */

//// This function converts from Polar Coordinates to Cartesian coordinates

vec2 polar2cart(float angle, float length)
{
    return vec2(cos(angle) * length, sin(angle) * length);
}

//// This is a sample function showing you how to check if a point is inside a triangle

bool inTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3)
{
    if(dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p1, 0))) >= 0. &&
        dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p1 - p2, 0))) >= 0. &&
        dot(cross(vec3(p1 - p3, 0), vec3(p - p3, 0)), cross(vec3(p1 - p3, 0), vec3(p2 - p3, 0))) >= 0.){
        return true;
    }
    return false;
}

//// This is a sample function showing you how to draw a rotated triangle 
//// Time is specified with iTime

vec3 drawTriangle(vec2 pos, vec2 center, vec3 color)
{
    vec2 p1 = polar2cart(iTime * 2, 160.) + center;
    vec2 p2 = polar2cart(iTime * 2 + 2. * M_PI / 3., 160.) + center;
    vec2 p3 = polar2cart(iTime * 2 + 4. * M_PI / 3., 160.) + center;
    if(inTriangle(pos, p1, p2, p3)){
        return color;
    }
    return vec3(0);
}

// NEW FUNCTION FOR A STILL TRIANGLE
vec3 drawStillTriangle(vec2 pos, vec2 p1, vec2 p2, vec2 p3, vec3 color) 
{
    if(inTriangle(pos, p1, p2, p3)){
        return color;
    }
    return vec3(0);
}

/////////////////////////////////////////////////////
//// Step 1 Function: Inside Circle
//// In this function, you will implement a function to checks if a point is inside a circle
//// The inputs include the point position, the circle's center and radius
//// The output is a bool indicating if the point is inside the circle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use dot(v,v) to calculate the squared length of a vector v
/////////////////////////////////////////////////////

bool inCircle(vec2 pos, vec2 center, float radius)
{
    /* your implementation starts */
    vec2 cenToPos = vec2(pos - center);
    if(dot(cenToPos, cenToPos) <= radius * radius){
        return true;
    }
    /* your implementation ends */
    
    return false;
}

//// This function calls the inCircle function you implemented above and returns the color of the circle
//// If the point is outside the circle, it returns a zero vector by default
vec3 drawCircle(vec2 pos, vec2 center, float radius, vec3 color)
{
    if(inCircle(pos, center, radius)){
        return color;
    }
    return vec3(0);
}

// FUNCTION FOR DRAWING AN OVAL
bool inOval(vec2 pos, vec2 center, float radiusX, float radiusY)
{
    vec2 cenToPos = vec2(pos - center);
    if(((cenToPos.x * cenToPos.x) / (radiusX * radiusX)) 
    + ((cenToPos.y * cenToPos.y) / (radiusY * radiusY)) <= 1) {
        return true;
    }
    return false;
}


vec3 drawOval(vec2 pos, vec2 center, float radiusX, float radiusY, vec3 color)
{
    if(inOval(pos, center, radiusX, radiusY)){
        return color;
    }
    return vec3(0);
}

/////////////////////////////////////////////////////
//// Step 2 Function: Inside Rectangle
//// In this function, you will implement a function to checks if a point is inside a rectangle
//// The inputs include the point position, the left bottom corner and the right top corner of the rectangle
//// The output is a bool indicating if the point is inside the rectangle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use .x and .y to access the x and y components of a vec2 variable
/////////////////////////////////////////////////////

bool inRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop)
{
    /* your implementation starts */
    if(pos.x >= leftBottom.x &&
        pos.x <= rightTop.x &&
        pos.y >= leftBottom.y &&
        pos.y <= rightTop.y) {
            return true;
        }
    /* your implementation ends */
    
    return false;
}

//// This function calls the inRectangle function you implemented above and returns the color of the rectangle
//// If the point is outside the rectangle, it returns a zero vector by default

vec3 drawRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop, vec3 color)
{
    if(inRectangle(pos,leftBottom,rightTop)){
        return color;
    }
    return vec3(0);
}

// FUNCTIONS FOR DRAWING TRAPEZOID!!
bool inTrapezoid(vec2 pos, vec2 topLeft, vec2 topRight, vec2 bottomLeft, vec2 bottomRight)
{
    if(inTriangle(pos, bottomLeft, topLeft, topRight) || inTriangle(pos, bottomLeft, topRight, bottomRight)){
        return true;
    }
    return false;
}

vec3 drawTrapezoid(vec2 pos, vec2 topLeft, vec2 topRight, vec2 bottomLeft, vec2 bottomRight, vec3 color)
{
    if(inTrapezoid(pos, topLeft, topRight, bottomLeft, bottomRight)){
        return color;
    }
    return vec3(0);
}

//// This function draws objects on the canvas by specifying a fragColor for each fragCoord

void mainImage(in vec2 fragCoord, out vec4 fragColor)
{
    // // OOOOO TESTING OUT GRADIENTS!!
    // fragColor = vec4(fragCoord.y/1000.0, fragCoord.x/1000.0, 0.0, 1.0);
    
    //// Get the window center
    vec2 center = vec2(iResolution / 2.);

    //// By default we draw an animated triangle 
    vec3 fragOutput = drawTriangle(fragCoord, center, vec3(1.0));
    
    //// Step 1: Uncomment this line to draw a circle
    fragOutput = drawCircle(fragCoord, center, 250, vec3(1.0));

    
    //// Step 2: Uncomment this line to draw a rectangle 
    fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0));


    //// Step 3: Uncomment this line to draw an animated circle with a temporally varying radius controlled by a sine function
    fragOutput = drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    //// Step 4: Uncomment this line to draw a union of the rectangle and the animated circle you have drawn previously
    fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0)) + drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    // MY CREATIVE EXPRESSION
    // BACKGROUND
    vec3 skyTop = drawRectangle(fragCoord, vec2(0), vec2(iResolution), vec3(210 - (fragCoord.y / 5), 134 - (fragCoord.y / 9), 225 - (fragCoord.y / 8)) / 255.);
    vec3 moon = drawCircle(fragCoord, vec2(350, 700), 60., vec3(1));
    vec3 star1 = drawCircle(fragCoord, vec2(150, 830), 5., vec3(1));
    vec3 star2 = drawCircle(fragCoord, vec2(75, 500), 5., vec3(1));
    vec3 star3 = drawCircle(fragCoord, vec2(600, 650), 5., vec3(1));
    vec3 star4 = drawCircle(fragCoord, vec2(700, 850), 5., vec3(1));
    vec3 star5 = drawCircle(fragCoord, vec2(900, 500), 5., vec3(1));
    vec3 star6 = drawCircle(fragCoord, vec2(1100, 800), 5., vec3(1));
    vec3 star7 = drawCircle(fragCoord, vec2(1200, 600), 5., vec3(1));
    vec3 star8 = drawCircle(fragCoord, vec2(1400, 800), 5., vec3(1));
    vec3 star9 = drawCircle(fragCoord, vec2(350, 430), 5., vec3(1));
    vec3 star10 = drawCircle(fragCoord, vec2(1310, 480), 5., vec3(1));

    vec3 background = moon + skyTop + star1 + star2 + star3 + star4 + star5 + star6 + star7 + star8 + star9 + star10;

    // MOUNTAINS
    vec3 bLeftMtn = drawStillTriangle(fragCoord, vec2(0), vec2(0, 400), vec2(500, 0), vec3(86, 75, 143) / 255.);
    vec3 bMidMtn = drawStillTriangle(fragCoord, vec2(100, 0), vec2(700, 560), vec2(1300, 0), vec3(66, 55, 123) / 255.);
    vec3 bRightMtn = drawStillTriangle(fragCoord, vec2(900, 0), vec2(iResolution.x, 450), vec2(iResolution.x, 0), vec3(86, 75, 143) / 255.);

    vec3 backMountains = bLeftMtn + bMidMtn + bRightMtn;


    vec3 fLeftMtn = drawStillTriangle(fragCoord, vec2(0, 300), vec2(300, 500), vec2(1000, 0), vec3(140, 128, 198) / 255.);
    vec3 fRightMtn = drawStillTriangle(fragCoord, vec2(600, 0), vec2(1150, 450), vec2(iResolution.x, 200), vec3(140, 128, 198) / 255.);
    vec3 leftBottom = drawStillTriangle(fragCoord, vec2(0), vec2(0, 300), vec2(1000,0), vec3(140, 128, 198) / 255.);
    vec3 rightBottom = drawStillTriangle(fragCoord, vec2(600, 0), vec2(iResolution.x, 200), vec2(iResolution.x,0), vec3(140, 128, 198) / 255.);

    vec3 leftFrontMountain = fLeftMtn + leftBottom;
    vec3 rightFrontMountain = fRightMtn + rightBottom;

    //BLACK SILHOUETTE
    vec3 hillTop = drawOval(fragCoord, vec2(iResolution.x / 2, 50), iResolution.x / 2 + 200, 120, vec3(1) / 255.);
    vec3 treeTrnk = drawTrapezoid(fragCoord, vec2(1090, 470), vec2(1115, 470), vec2(1085, 100), vec2(1120, 100), vec3(1) / 255.);
    // vec3 treeTop = drawTrapezoid(fragCoord, vec2(1080, 480), vec2(1125, 480), vec2(1090, 470), vec2(1115, 470), vec3(1) / 255.);
    // vec3 leaf1 = drawStillTriangle(fragCoord, vec2(870, 420), vec2(950, 490), vec2(1102, 470), vec3(1) / 255.);
    // vec3 leaf2 = drawStillTriangle(fragCoord, vec2(900, 545), vec2(1000, 565), vec2(1102, 470), vec3(1) / 255.);
    // vec3 leaf3 = drawStillTriangle(fragCoord, vec2(1102, 460), vec2(1120, 590), vec2(1180, 620), vec3(1) / 255.);
    // vec3 leaf4 = drawStillTriangle(fragCoord, vec2(1102, 471), vec2(1200, 520), vec2(1300, 490), vec3(1) / 255.);
    // vec3 leaf5 = drawStillTriangle(fragCoord, vec2(1102, 471), vec2(1200, 460), vec2(1300, 390), vec3(1) / 255.);

    vec3 branch1 = drawStillTriangle(fragCoord, vec2(1090, 430), vec2(1090, 460), vec2(990, 510), vec3(1) / 255.);
    vec3 branch2 = drawStillTriangle(fragCoord, vec2(1090, 470), vec2(1115, 470), vec2(1110, 570), vec3(1) / 255.);
    vec3 branch3 = drawStillTriangle(fragCoord, vec2(1115, 400), vec2(1115, 430), vec2(1220, 470), vec3(1) / 255.);
    vec3 branch4 = drawStillTriangle(fragCoord, vec2(1090, 390), vec2(1090, 412), vec2(980, 440), vec3(1) / 255.);

    vec3 sBranch1 = drawStillTriangle(fragCoord, vec2(1055, 470), vec2(1075, 460), vec2(1040, 540), vec3(1) / 255.);
    vec3 sBranch2 = drawStillTriangle(fragCoord, vec2(1125, 410), vec2(1160, 430), vec2(1190, 500), vec3(1) / 255.);
    vec3 sBranch3 = drawStillTriangle(fragCoord, vec2(1130, 410), vec2(1140, 430), vec2(1190, 400), vec3(1) / 255.);
    vec3 sBranch4 = drawStillTriangle(fragCoord, vec2(1050, 420), vec2(1070, 410), vec2(1020, 470), vec3(1) / 255.);

    vec3 blackSilhouette = hillTop + treeTrnk + branch1 + branch2 + branch3 + branch4 + sBranch1 + sBranch2 + sBranch3 + sBranch4;

    //TREE LEAVES
    vec3 leafCol = vec3(25, 14, 71) / 255.;
    vec3 leaf1 = drawOval(fragCoord, vec2(1000, 500), 120, 110, leafCol);
    vec3 leaf2 = drawOval(fragCoord, vec2(1100, 550), 130, 135, leafCol);
    vec3 leaf3 = drawOval(fragCoord, vec2(1200, 450), 130, 110, leafCol);
    vec3 leaf4 = drawOval(fragCoord, vec2(1050, 420), 150, 70, leafCol);

    vec3 leaves = leaf1 + leaf2 + leaf3 + leaf4;


    fragOutput = background + backMountains + leftFrontMountain + rightFrontMountain + blackSilhouette;
    if(blackSilhouette != vec3(0)) {
        fragColor = vec4(blackSilhouette, 1.0);
    }else if(leaves != vec3(0)) {
        fragColor = vec4(leafCol, 1.0);
    }else if(leftFrontMountain != vec3(0)){
        fragColor = vec4(leftFrontMountain, 1.0);
    }else if(rightFrontMountain != vec3(0)){
        fragColor = vec4(rightFrontMountain, 1.0);
    }else if(backMountains != vec3(0)){
        fragColor = vec4(backMountains, 1.0);
    } else if(fragOutput != vec3(0)){
        fragColor = vec4(fragOutput, 1.0);
    } else{
        fragColor = vec4(BG_COLOR, 1.0);
    }
    // //// Set the fragment color to be the background color if it is zero
    // if(fragOutput == vec3(0)){
    //     fragColor = vec4(BG_COLOR, 1.0);
    // }
    // //// Otherwise set the fragment color to be the color calculated in fragOutput
    // else{
    //     fragColor = vec4(fragOutput, 1.0);
    // }

    //// Step 5: Implement your customized scene by modifying the mainImage function
    //// Try to leverage what you have learned from Step 1 to 4 to define the shape and color of a new object in the fragment shader
    //// Notice how we put multiple objects together by adding their color values
}

void main()
{
    mainImage(fragCoord, fragColor);
}
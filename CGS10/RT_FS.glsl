#version 120

#define NUM_PLANES 1 // 1 Ground plane
#define NUM_SPHERES 4 // Sun, Earth, Moon, Saturn

#define NUM_MAX_INTERSECTIONS (NUM_PLANES + NUM_SPHERES)
#define NUM_MAX_REFLECTIONS 16
#define EPSILON 0.002
#define INFINITY 1000

// Object IDs
#define OBJ_ID_SUN 0
#define OBJ_ID_EARTH 1
#define OBJ_ID_MOON 2
#define OBJ_ID_SATURN 3
#define OBJ_ID_GROUND_PLANE NUM_SPHERES

// Uniform variables
uniform vec2 uResolution;
uniform float uFov;
uniform vec3 uUp;
uniform vec3 uDirection;
uniform vec3 uPosition;

// XYZ - center of the sphere
// W - radius of the sphere
uniform vec4 uSphereData[NUM_SPHERES];

// Common data types
struct Ray {
	vec3 origin;
	vec3 direction;
};

struct IntersectionTestResult {
	bool isIntersection;
	float tHit;
};

struct SceneIntersectionTestResult {
	IntersectionTestResult intersection;
	int objectId;
	vec3 normal;
	vec3 hitPosition;
};

const IntersectionTestResult noIntersection = IntersectionTestResult(false, 0);
const SceneIntersectionTestResult noSceneIntersection = SceneIntersectionTestResult(noIntersection, -1, vec3(0), vec3(0));

// Intersection tests
IntersectionTestResult intersectRaySphere(Ray ray, vec4 sphereData) {
	// TODO: a) Ray-Sphere-Intersection
	// Implement a ray sphere intersection test here
    
    vec3 l = sphereData.xyz - ray.origin;

    float lengthR = length(ray.direction);
    float lengthL = length(l);

    float cosa = dot(ray.direction, l) / (lengthR * lengthL);
    float sina = sqrt(1.0 - (cosa * cosa));

    float x = sina * lengthL;

    float r = sphereData.w;
    if (x <= r)
    {
        float y = sqrt((sphereData.w * sphereData.w) - (x*x));
        float g = sqrt((lengthL*lengthL) - (x*x));
        
        return IntersectionTestResult(true, g-y);
    }

    return noIntersection;
}

// Ray plane intersection test
IntersectionTestResult intersectRayPlane(Ray ray, vec4 planeData) {
	float v = -dot(ray.direction, planeData.xyz);
	float tHit = (dot(ray.origin, planeData.xyz) - planeData.w) / v;
	return IntersectionTestResult(tHit > 0, tHit);
}

vec3 getNormal(int objectId, vec3 hitPosition) {
	if (objectId < OBJ_ID_GROUND_PLANE) { // SPHERE
		return normalize(hitPosition - uSphereData[objectId].xyz);
	} else if (objectId == OBJ_ID_GROUND_PLANE) { // PLANE
		return vec3(0,1,0);
	}
}


// Returns the closest intersection of the ray with the scene
SceneIntersectionTestResult intersectRayScene(Ray ray) {
	// Dummy scene intersection - you have to remove this
//--- DUMMY CODE BEGIN ---
	// IntersectionTestResult result = intersectRayPlane(ray, vec4(0,1,0,-30));
	// // No intersection occurred
	// if (!result.isIntersection)
	// 	return noSceneIntersection;
	// // Compute normal and the position of the intersection
	// vec3 hitPosition = ray.origin + result.tHit * ray.direction;
	// vec3 normal = getNormal(OBJ_ID_GROUND_PLANE, hitPosition);
	// 
	// // Epsilon correction for the hitposition
	// hitPosition += normal * EPSILON;
	// return SceneIntersectionTestResult(result, OBJ_ID_GROUND_PLANE, normal, hitPosition);
//--- DUMMY CODE END ---
	// TODO: b) Ray-Scene-Intersection
	// TODO: Rewrite your code here
	// TODO: Perform intersection tests
    IntersectionTestResult results[NUM_MAX_INTERSECTIONS];
    // For all spheres
    for (int i = 0; i < NUM_SPHERES; i++)
        results[i] = intersectRaySphere(ray, uSphereData[i]);
    
    // For the ground plane
    results[NUM_SPHERES] = intersectRayPlane(ray, vec4(0,1,0,-30));
    
    // Find the closest intersection along the ray
    int minIndex = -1;
    
    for (int i = 0; i < NUM_MAX_INTERSECTIONS; i++)
    {
        if (results[i].isIntersection)
        {
            minIndex = i;
            break;
        }
    }

    if (minIndex == -1)
        return noSceneIntersection;
    
    for (int i = minIndex + 1; i < NUM_MAX_INTERSECTIONS; i++)
    {
        if (results[i].tHit < results[minIndex].tHit && results[i].isIntersection)
        {
            minIndex = i;
        }
    }
    
    if (!results[minIndex].isIntersection)
        return noSceneIntersection;
    
    // Compute normal and the position of the intersection
    vec3 hitPosition = ray.origin + results[minIndex].tHit * ray.direction;
    vec3 normal = getNormal(minIndex, hitPosition);
    
    // Epsilon correction for the hitposition
    hitPosition += normal * EPSILON;
    
    // Return the closest intersection
    return SceneIntersectionTestResult(results[minIndex], minIndex, normal, hitPosition);
}

// Shadow ray intersection test
bool intersectRaySceneShadow(Ray ray) {
	// Perform intersection tests
	IntersectionTestResult results[NUM_MAX_INTERSECTIONS];
	for (int i = 0; i < NUM_SPHERES; ++i)
		results[i] = intersectRaySphere(ray, uSphereData[i]);
	results[NUM_SPHERES] = intersectRayPlane(ray, vec4(0,1,0,-30));
	
	float tHitLimit = min(results[0].tHit, INFINITY);
	
	for (int i = 1; i < NUM_MAX_INTERSECTIONS; ++i) {
		if (results[i].isIntersection && results[i].tHit < tHitLimit) {
			return true;
		}
	}

	return false;
}

vec4 smoothCheckerboard(vec2 texCoord) {
	float fq = 0.5;
	vec2 w = fwidth(texCoord.xy);
	vec2 fu = w * fq * 2;
	float fum = max(fu.x, fu.y);
	vec2 cp = fract(texCoord.xy * fq);

	vec2 pp = smoothstep(vec2(.5), fu + vec2(.5), cp) +	(1.0 - smoothstep(vec2(0.0), fu, cp));
	vec4 c0 = vec4(1);
	vec4 c1 = vec4(0);
	vec4 ca = c0 * .5 + c1 * .5;

	vec4 cc = mix(c0, c1, pp.x * pp.y + (1 - pp.y) * (1 - pp.x));
	cc = mix(cc, ca, smoothstep(.125, .75, fum));
	return cc;
}

// Computes the local color at a hitpoint
vec4 getColor(SceneIntersectionTestResult result, Ray ray) {
	vec3 toLightP = normalize(vec3(0) - result.hitPosition);
	vec3 N = result.normal;
	float nDotLP = max(0, dot(N, toLightP));
	float nDotLD = max(0, dot(N, vec3(0.0,0.0,-1.0)));

	vec4 baseColor = vec4(1.0,1.0,1.0,1.0);

	if (result.objectId == OBJ_ID_SUN) {
		baseColor = vec4(1.,1.,0.8,1.);
	} else if (result.objectId == OBJ_ID_EARTH) {
		baseColor = vec4(.3,.7,1.,1.);
	} else if (result.objectId == OBJ_ID_MOON) {
		baseColor = vec4(.5);
	} else if (result.objectId == OBJ_ID_SATURN) {
		baseColor = vec4(1.,1.,0.3,1.);
	}

	vec4 color = (nDotLP + nDotLD) * baseColor;
	color += pow(max(dot(result.normal ,normalize(toLightP - ray.direction)), 0), 40.);

	if (result.objectId == OBJ_ID_GROUND_PLANE)
		color *= (smoothCheckerboard(result.hitPosition.xz*0.1)*.5+.5);

	// TODO: c) Shadow-Test
	// Implement a shadow test here (the direction to the lightsource is toLightP)

	return color;
}

// Entry point
void main() {
	// Camera setup
	vec3 nD = normalize(uDirection.xyz);
	vec3 nS = normalize(cross(nD, uUp.xyz));
	vec3 nUp = cross(nS, nD);
	mat3 M = mat3(nS, nUp, -nD);

	// Setup primary ray
	Ray primaryRay;
	primaryRay.origin = uPosition.xyz;
	primaryRay.direction =  M *
		normalize(vec3((gl_FragCoord.xy-vec2(.5*uResolution.xy))/uResolution.y,-.5/tan(.5*3.1415*uFov/180.)));

	// Intersect primary ray with the scene
	SceneIntersectionTestResult r = intersectRayScene(primaryRay);
	
	// Shade the background white in case nothing was hit
	if (r.objectId == -1) {
		gl_FragColor = vec4(1.0,1.0,1.0,1.0);
		return;
	}

	// Compute color at the first hitpoint
	gl_FragColor = getColor(r, primaryRay);

	// TODO: d) Reflections
	// TODO: Compute reflections for sun & earth
	Ray reflectionRay = primaryRay;

}

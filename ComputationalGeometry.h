#ifndef COMPUTATIONAL_GEOMETRY_H
#define COMPUTATIONAL_GEOMETRY_H
#include"Mat.h"
/*--------------------------------[ CircumCircle 三角形外接圆 ]--------------------------------
*	三角形外接圆圆心, 是任意两边的垂直平分线的交点.
*	正弦定理: a/sinA = b/sinB = c/sinC = 2R
*	半径,在求得圆心坐标后，直接用距离公式。
**------------------------------------------------------------------*/
double CircumCircle(Mat<double>& triangle, Mat<double>& center)
{
	Mat<double> point[3];
	for (int i = 0; i < 3; i++) triangle.getCol(i, point[i]);
	//三角形外接圆圆心
	center.zero(triangle.rows, 1);
	double t[3];
	for (int i = 0; i < 3; i++) t[i] = x[i] * x[i] + y[i] * y[i];
	double temp = (x[2] - x[1]) * (y[1] - y[0]) - (x[1] - x[0]) * (y[2] - y[1]);
	center[0] = (-(t[1] - t[0]) * (y[2] - y[1]) + (t[2] - t[1]) * (y[1] - y[0])) / (2 * temp);
	center[1] = (+(t[1] - t[0]) * (x[2] - x[1]) - (t[2] - t[1]) * (x[1] - x[0])) / (2 * temp);
	//半径
	point[0].add(point[0].negative(point[0]), center);
	return point[0].norm();
}
/*--------------------------------[ CrossProduct 叉乘 ]--------------------------------
*	利用叉积，可以判断两条向量之间的旋转方向
*	𝑎 × 𝑏 ⃑ = | 𝑥		𝑦	𝑧 | = z ∙ (𝑥𝑎 ∙ 𝑦𝑏 − 𝑥𝑏 ∙ 𝑦𝑎)
				| 𝑥𝑎	𝑦𝑎	0 |
				| 𝑥𝑏	𝑦𝑏	0 |
**---------------------------------------------------------*/
double CrossProduct(Mat<double>& a, Mat<double>& b)
{
	return a[0] * b[1] - a[1] * b[0];
}
bool OnSegments_judge(segment seg, point point)
{
	seg.p[0].x += -point.x + seg.p[0].y - point.y;
	seg.p[1].x += -point.x + seg.p[1].y - point.y;
	if (seg.p[0].x * seg.p[1].x <= 0)return true;
	return false;
}
/*--------------------------------[ Segments Intersect 线段相交判断 ]--------------------------------
*	判定条件：
	1.Each segment straddles the line containing the other.
	2.An endpoint of one segment line on the other segment. (the boundary case.)
**---------------------------------------------------------*/
bool isSegmentsIntersect(Mat<double>& a, Mat<double>& b)
{
	double dir_a1 = CrossProduct(a.p[0], a.p[1], b.p[0]),
		dir_a2 = CrossProduct(a.p[0], a.p[1], b.p[1]),
		dir_b1 = CrossProduct(b.p[0], b.p[1], a.p[0]),
		dir_b2 = CrossProduct(b.p[0], b.p[1], a.p[1]);
	if (dir_a1 == 0)
		if (OnSegments_judge(a, b.p[0])) return true; else {}
	else if (dir_a2 == 0)
		if (OnSegments_judge(a, b.p[1])) return true; else {}
	else if (dir_b1 == 0)
		if (OnSegments_judge(b, a.p[0])) return true; else {}
	else if (dir_b2 == 0)
		if (OnSegments_judge(b, a.p[1])) return true; else {}
	else if (dir_a1 != dir_a2 && dir_b1 != dir_b2) return true;
	return false;
}
/*--------------------------------[ ConvexHull 凸包 ]--------------------------------
*	[算法]: Graham 扫描法
*	[流程]:
		[1] 选择y最小的点 p0,若多个则选其中最靠左的点
		[2] 根据相对p0的极角,对剩余点排序
**----------------------------------------------------------------------------*/
void ConvexHull(Mat<double>& points)
{
	Mat<double> Stack(points.rows, points.cols);
	int StackPos = 0;
	for (int i = 0; i < points.cols; i++) {
		if (i == cur)continue;
		while (StackPos == 0) {
			position prePoint = ansPoint.top();
			ansPoint.pop();
			if (ansPoint.empty()) {
				ansPoint.push(prePoint);
				ansPoint.push(arr[i]);
				break;
			}
			double ax = prePoint.x - ansPoint.top().x;
			double ay = prePoint.y - ansPoint.top().y;
			double bx = arr[i].x - prePoint.x;
			double by = arr[i].y - prePoint.y;
			if (CrossProduct(a, b)) {
				ansPoint.push(prePoint);
				ansPoint.push(arr[i]);
				break;
			}
		}
		if (ansPoint.empty())ansPoint.push(arr[i]);
	}ansPoint.push(arr[cur]);
}
/*--------------------------------[ Delaunay 三角剖分 ]--------------------------------
*	[定义]:
		[1] Delaunay三角剖分: 每个三角形的外接圆内不包含V中任何点
	[流程]:
　　　　初始化顶点列表
　　　　创建索引列表(indices = new Array(vertices.length))　　　　//indices数组中的值为0,1,2,3,......,vertices.length-1
　　　　基于vertices中的顶点x坐标对indices进行sort　　  　　　　　  //sort后的indices值顺序为顶点坐标x从小到大排序（也可对y坐标，本例中针对x坐标）
　　　　确定超级三角形
　　　　将超级三角形保存至未确定三角形列表（temp triangles）
　　　　将超级三角形push到triangles列表
　　　　遍历基于indices顺序的vertices中每一个点　　　　　　　　　  　//基于indices后，则顶点则是由x从小到大出现
　　　　　　初始化边缓存数组（edge buffer）
　　　　　　遍历temp triangles中的每一个三角形
　　　　　　　　计算该三角形的圆心和半径
　　　　　　　　如果该点在外接圆的右侧
　　　　　　　　　　则该三角形为Delaunay三角形，保存到triangles
　　　　　　　　　　并在temp里去除掉
　　　　　　　　　　跳过
　　　　　　　　如果该点在外接圆外（即也不是外接圆右侧）
　　　　　　　　　　则该三角形为不确定        　　　　　　　　　     //后面会在问题中讨论
　　　　　　　　　　跳过
　　　　　　　　如果该点在外接圆内
　　　　　　　　　　则该三角形不为Delaunay三角形
　　　　　　　　　　将三边保存至edge buffer
　　　　　　　　　　在temp中去除掉该三角形
　　　　　　对edge buffer进行去重
　　　　　　将edge buffer中的边与当前的点进行组合成若干三角形并保存至temp triangles中
　　　　将triangles与temp triangles进行合并
　　　　除去与超级三角形有关的三角形
**----------------------------------------------------------------------*/
struct triangle { position a, b, c; };
triangle trians[10 * MAXN + 1], tritemp[10 * MAXN + 1];
int trianspos = 0, tritemppos = 0;
void Delaunay(position p)
{
	edgetemp.clear();
	for (int i = 0; i < tritemppos; i++) {
		if (judge(tritemp[i], p) == 1) {
			trians[trianspos++] = tritemp[i];
			deletearr(tritemp, i, tritemppos);
			tritemppos--; i--;
		}
		else if (judge(tritemp[i], p) == 0) {
			edge temp[3] = { { tritemp[i].a, tritemp[i].b } ,
							 { tritemp[i].a, tritemp[i].c } ,
							 { tritemp[i].b, tritemp[i].c } };
			for (int j = 0; j < 3; j++) edgetemp.push_back(temp[j]);
			deletearr(tritemp, i, tritemppos);
			tritemppos--; i--;
		}
		else {}
	}
	sort(edgetemp.begin(), edgetemp.end());
	for (int i = 0; i < edgetemp.size() - 1; i++) {
		int flag = 0;
		while (i + 1 < edgetemp.size() && edgetemp[i] == edgetemp[i + 1]) {
			flag = 1;
			edgetemp.erase(edgetemp.begin() + i + 1);
		}
		if (flag) {
			edgetemp.erase(edgetemp.begin() + i); i--;
		}
	}
	for (int i = 0; i < edgetemp.size(); i++)
		tritemp[tritemppos++] = { edgetemp[i].a, edgetemp[i].b, p };
}
triangle SuperTriangle(position arr[], int N)
{
	position mincur = { arr[0].x ,99999 },
		maxcur = { arr[N - 1].x ,-99999 };
	for (int i = 1; i < N; i++) {
		if (arr[i].y < mincur.y) mincur.y = arr[i].y;
		if (arr[i].y > maxcur.y) maxcur.y = arr[i].y;
	}
	position length = { maxcur.x - mincur.x , maxcur.y - mincur.y };
	double mid_x = (maxcur.x + mincur.x) / 2;
	position a = { mincur.x - length.x - 2,mincur.y - 2 },
		b = { maxcur.x + length.x + 2,mincur.y - 2 },
		c = { mid_x ,maxcur.y + length.y + 2 };
	tritemp[tritemppos++] = { a,b,c };
	triangle supertriangle = { a,b,c };
	return supertriangle;
}
int judge(triangle tri, position point)
{
	double x[3] = { tri.a.x, tri.b.x, tri.c.x };
	double y[3] = { tri.a.y, tri.b.y, tri.c.y };
	double a = sqrt(pow((x[0] - x[1]), 2) + pow((y[0] - y[1]), 2));
	double b = sqrt(pow((x[0] - x[2]), 2) + pow((y[0] - y[2]), 2));
	double c = sqrt(pow((x[1] - x[2]), 2) + pow((y[1] - y[2]), 2));
	double p = (a + b + c) / 2;
	double S = sqrt(p * (p - a) * (p - b) * (p - c));
	double radius = a * b * c / (4 * S);

	position center;
	double t[3];
	for (int i = 0; i < 3; i++) {
		t[i] = x[i] * x[i] + y[i] * y[i];
	}
	double temp = (x[2] - x[1]) * (y[1] - y[0]) - (x[1] - x[0]) * (y[2] - y[1]);
	center.x = (-(t[1] - t[0]) * (y[2] - y[1]) + (t[2] - t[1]) * (y[1] - y[0])) / (2 * temp);
	center.y = (+(t[1] - t[0]) * (x[2] - x[1]) - (t[2] - t[1]) * (x[1] - x[0])) / (2 * temp);
	if (point.x > center.x + radius)return 1;
	double s = sqrt(pow((point.x - center.x), 2) + pow((point.y - center.y), 2));
	if (s < radius)return 0;
	return -1;
}
void deletearr(triangle arr[], int cur, int N)
{
	for (int i = cur + 1; i < N; i++)arr[i - 1] = arr[i];
}
bool supertriangle_judge(triangle t1, triangle t2)
{
	position p[6] = { t1.a,t1.b,t1.c,t2.a,t2.b,t2.c };
	sort(p, p + 6);
	for (int i = 0; i < 5; i++)
		if (p[i] == p[i + 1])
			return true;
	return false;
}
vector<edge> edgetemp;


#endif
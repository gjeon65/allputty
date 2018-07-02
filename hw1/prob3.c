int printf(const char *format, ...);
int main()
{
int n;
long int x,*p,*p1;
p=&x;
p1=(p+1);

printf("size of long int is : %d\n",n=(char *)(p1)-(char *)p);
}

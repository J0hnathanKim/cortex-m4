<11/11>  
Exception: 예외처리 프로그램(서비스 루틴 또는 헨들러)  
cpu <-  버스 -> memory  
usage fault: 명령어 실행 관련된 오류(컴파일러)  
ISR: 인터럽트 서비스 루틴  
normal program --(INT)------------------>ISR  
vector table: ISR의 위치(고정 위치) (program -> vector table -> ISR)  
126개의 인터럽스 소스 존재  
<인터럽트 컨트롤러>  
1)인터럽트 발생(하드웨어 인터럽트: 전기적 신호, 소프트웨어 인터럽트: 명령어) -> 인터럽트 소스  
2)진짜 인터럽트신호인지 체크(INT Check)  
3)인터럽트 enable  
4)인터럽트 masking  
5)우선순위 체크  
6)vecotr table  
7)ISR  
<레지스터>  
-mask  
<인터럽트 서비스 루틴>  
<LCD>  
display site: 480 x 272  
cpu -> frame buffer (buffer memory), pointer, video controller, dma  
data  
1)image  
카메라로 찍어서 생성된 데이터  
2)graphics  
픽셀 단위로 만들어짐  
pixel => gray(밝기, 0~255 byte)  
color(r, g, b)  
프로그램 실행 -> 코드 영역에 들어감(데이터 영역 x)  
code, buffer  
image_trans.exe -> data(image)를 buffer로 보낸다.  
<11/18>  
draw image -> scan line(좌표 평면위의 각 줄)  
좌표계산에 익숙해질것.  
맨 왼쪽 위가 0,0 ? 맨 오른쪽 아래가 480,272?  
중심(240, 136)  
data -> bit map(memory) -> buffer(frame buffer) / LCD제어기 -> 화면에 이미지 뿌려줌  
색상: 밝기값을 얼마나 주냐 ex) 빨간색(5bit)에 11111값을 주면 제일 빨간색이 나옴.  
수평주파수(Hclk)  
draw circle => 원의 방정식 활용. 원의 범위 내에서 픽셀을 찍도록 구현  
draw triangle?  




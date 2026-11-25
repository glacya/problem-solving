# problem-solving

BOJ 문제를 고르고, VSCode로 풀고, 제출하고, AC 기록을 커밋하는 개인용 웹 페이지.

## 필요한 것

- Linux 또는 WSL
- `g++` (C++20), `python3` 3.11+ (venv, pip 사용 가능), `curl`, `git`
- [solved.ac API 키](https://backstage.solved.ac/ko/developer/apps): 앱을 등록해 받는다.
- git remote와 push 인증(PAT 또는 credential helper). 서버는 인증을 물어볼 수 없다.
- Node.js는 따로 설치하지 않아도 된다. `setup.sh`가 `.tools/node`에 받는다.

## 설치

```bash
cp .env.example .env    # SOLVEDAC_CLIENT_ID, SOLVEDAC_CLIENT_SECRET, BOJ_HANDLE 채우기
./setup.sh              # .venv 생성, 패키지 설치, 프론트엔드 빌드
```

## 사용법

```bash
./ps.sh                 # 서버를 띄우고 브라우저로 연다 (http://127.0.0.1:8710)
./ps.sh stop            # 끄기
./ps.sh restart         # 다시 띄우기
./ps.sh status          # 실행 중인지
./ps.sh logs            # 로그 보기
```

1. **홈**에서 문제를 뽑거나, 풀다 만 문제 / 나중에 풀기 / 대기 중인 문제를 고른다. 안 푼 문제의 티어와 태그는 보이지 않는다.
2. **C++로 풀기** 또는 **Python으로 풀기**를 누르면 `problems/<번호>/main.cpp`(또는 `main.py`)가 VSCode에서 열린다.
   - 지문은 `problem.md`, 예제는 `inputs/`, `outputs/`에 있다.
   - 직접 만든 테스트는 `inputs/이름.in`(정답은 `outputs/이름.out`)에 넣는다.
   - 메모는 문제 페이지 옆에 쓴다.
   - 공용 코드는 `lib/`에 두고 `#include "x.cpp"` 또는 `import x`로 쓴다.
3. **제출 준비** 페이지에서 순서대로 진행한다.
   1. 테스트를 실행한다.
   2. 제출 코드를 확인한다.
   3. "제출본 저장 · 복사 · 제출 페이지 열기"를 누르고, BOJ에서 붙여넣어 제출한다.
4. AC가 확인되면 `solution/boj/<번호>.<cpp|py>`와 `history/YYYY-MM.md`가 커밋되고 push된다.
   - 서버가 solved.ac에서 자동으로 확인한다.
   - BOJ에서 직접 확인했다면 **직접 AC 처리**를 누른다.

### 예전 소스 모으기

이 도구 없이 푼 문제는 **소스 모으기** 페이지에서 하나씩 추가한다.

1. 페이지를 열면 solved.ac에서 푼 문제를 받아, `solution/boj/`에 소스가 없는 문제를 보여 준다. 몇 초 걸린다.
2. 문제를 누르고, 푼 날짜·언어·코드를 넣는다. "BOJ 에서 내 맞은 제출 보기"로 제출 시각과 소스를 확인할 수 있다.
3. 저장하면 `solution/boj/<번호>.<확장자>`가 생기고 `history/YYYY-MM.md`가 갱신된다. 커밋은 하지 않으므로 직접 커밋한다.

AC를 받고 마지막 제출일로부터 3일이 지난 문제의 풀이 폴더(`problems/<번호>/`)는 서버가 자동으로 지운다(메모는 남긴다). **기록** 페이지에서 바로 정리할 수도 있다.

`./ps.sh`를 VSCode 터미널에서 실행하면 '풀기' 버튼이 그 VSCode에서 바로 파일을 연다.

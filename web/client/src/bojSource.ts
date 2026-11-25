// BOJ 소스 모으기 북마클릿. 해석은 서버(psweb/bojsource.py)가 한다.
//
// - 소스 페이지(/source/<제출 번호>)에서 누르면: 페이지 본문을 이 서버로 보내 바로 저장하고,
//   같은 탭을 다음 문제(소스가 없는 문제)의 "내 맞은 제출" 채점 현황으로 옮긴다.
// - 그 밖의 페이지에서 누르면: 다음 문제로 넘어간다. 채점 현황(?problem_id=)에서 누르면 그 문제를 건너뛴다.
// BOJ 에는 아무 요청도 하지 않는다. 사용자가 연 페이지를 읽고, 사용자가 누를 때 다음 페이지로 한 번 이동할 뿐이다.
// 서버는 BOJ origin 에서 오는 요청을 이 두 경로(/api/backfill/page, /api/backfill/next)에만 허용한다.

export function bookmarklet(origin: string) {
  const js = `(async()=>{
const O=${JSON.stringify(origin)};
const say=m=>{let d=document.getElementById('ps-toast');if(!d){d=document.createElement('div');d.id='ps-toast';d.style.cssText='position:fixed;top:16px;right:16px;z-index:99999;background:#1b1f24;color:#fff;padding:10px 14px;border-radius:8px;font:14px sans-serif;box-shadow:0 4px 16px rgba(0,0,0,.3);white-space:pre-line';document.body.appendChild(d)}d.textContent=m};
const go=n=>{if(!n){say('소스가 없는 문제가 더 없습니다');return}say(say.last+'다음: '+n.id+' '+n.title+' (남은 '+n.left+'문제)');location.href=n.statusUrl};
say.last='';
const j=async r=>{const b=await r.json().catch(()=>({}));if(!r.ok)throw new Error(b.detail||('HTTP '+r.status));return b};
try{
const t=document.querySelector('textarea[name=source]');
if(!t){const cur=+(new URLSearchParams(location.search).get('problem_id')||0);go((await j(await fetch(O+'/api/backfill/next?after='+cur))).next);return}
const html=(t.closest('.container')||document.body).outerHTML;
const post=async ok=>j(await fetch(O+'/api/backfill/page',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({html,confirm:ok})}));
say('저장하는 중…');
let r=await post(false);
if(r.confirm){if(!confirm(r.confirm.join('\\n')+'\\n그래도 저장할까요?')){say('저장하지 않았습니다');return}r=await post(true)}
say.last=r.exists?r.exists+'\\n':'저장: '+r.saved.id+' '+r.saved.problemTitle+' ('+r.saved.date+', '+r.saved.language+')\\n';
say(say.last);setTimeout(()=>go(r.next),600);
}catch(e){say('');alert('PS 소스 추가 실패: '+e.message+'\\n서버가 켜져 있는지, Chrome 이 묻는 로컬 네트워크 접근을 허용했는지 확인하세요.')}
})()`;
  return "javascript:" + encodeURIComponent(js.replace(/\n/g, ""));
}

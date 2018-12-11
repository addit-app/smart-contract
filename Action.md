# Action
## signup
> 사용자 가입
```
# action
@ name account : 가입자의 eos account
@ string nickname : 사용자 닉네임
@ string avatar : avatar image link
@ string memo : bio(자기 소개 등...)
cli push action eosadditapps signup '["violetstairn", "violetstair", "AVATAR1", "Hello"]' -p violetstairn@active

# get table
cli get table violetstairn violetstairn user
cli get table violetstairn violetstairs user
```

## addit
> 도메인에 대해 의견 등록
```
# action
@ name account : 게시자 eos account
@ int64_t iopinion : 평판할 사이트 index(처음 게시되는 사이트는 -1)
@ string url : 평판할 사이트 url(or hash)
# string comment : 평판 게시글
cli push action eosadditapps addit '["violetstairn", -1, "www.daum.net", "addit test"]' -p violetstairn@active
cli push action eosadditapps addit '["violetstairn", 2, "www.daum.net", "addit test"]' -p violetstairn@active


cli push action eosadditapps addit '["violetstairn", -1, "https://addit.io/", "addit test"]' -p violetstairn@active
cli push action eosadditapps addit '["violetstairs", 3, "https://addit.io/", "addit 조하요"]' -p violetstairs@active
cli push action eosadditapps addit '["violetstairn", 3, "https://addit.io/", "it just addit"]' -p violetstairn@active


# get table
# 평판 사이트 인덱스 조회(평판을 등록할 url의 인덱스 테이블)
cli get table eosadditapps eosadditapps domain

# 평판 사이트 코멘트(평판) 조회
cli get table eosadditapps 2 comments
cli get table eosadditapps 3 comments
```

## del
> 도메인에 대한 의견 삭제
```
# action 
@ name account : 삭제하려는 게시자의 eos account(작성자만 지울 수 있음)
@ uint64_t iopinion : 삭제하려는 게시물의 사이트 index
@ uint64_t icomment : 삭제하려는 comment의 index
cli push action eosadditapps del '["violetstairs", 2, 2]' -p violetstairs@active

# get table
cli get table eosadditapps 2 comments
```

## vote
> 도메인에 대한 의견(comment)에 대한 vote(좋아요/싫어요)
```
# action 
@ name account : voting 하려는 사용자의 eos account
@ uint64_t iopinion : 평판할 사이트의 index
@ uint64_t icomment : 평판할 코멘트의 index
@ int8_t vote : up/down vote(up : 1, down : -1)
cli push action eosadditapps vote '["violetstairs", 1, 0, 2]' -p violetstairs@active
cli push action eosadditapps vote '["violetstairn", 1, 0, 1]' -p violetstairn@active

# get table
cli get table eosadditapps violetstairn domain
cli get table eosadditapps 1 comments

```

## repute
> 사이트(URL)에대한 평판 조회(리워드 x)
```
# action 
@ name account : 평판을 등록하려는 사용자의 eos account
@ uint64_t iopinion : 평판을 등록하려는 도메인의 index
@ int8_t repute : up/down vote(up : 1, down : -1) 그외의 값은 에러
cli push action eosadditapps repute '["violetstairn", 3, 1]' -p violetstairn@active
cli push action eosadditapps repute '["violetstairs", 3, 1]' -p violetstairs@active
cli push action eosadditapps repute '["owdinnetwork", 3, 1]' -p owdinnetwork@active

# get table
# 평판 사이트 조회
cli get table eosadditapps 3 opinion
```

## create
> 유통시킬 token 생성
```
# action 
@ name issuer : 발행지(컨트랙트 소유자)
@ asset max_supply : 최대 발행 가능량
cli push action eosadditapps create '["violetstairn", "1000000000.0000 ADDIT"]' -p eosadditapps@active

# get table
cli get currency stats violetstairn ADDIT
```

## issue
> token 발행
```
# action 
@ asset quantity : 토큰 생성량
@ string memo : 메모
cli push action eosadditapps issue '["100000000.0000 ADDIT", "I LOVE VS"]' -p eosadditapps@active

# get table
cli get currency balance eosadditapps violetstairn ADDIT
cli get currency balance eosadditapps eosadditapps ADDIT
cli get currency balance eosadditapps violetstairs ADDIT
```

## transfer
> 토큰 전송
```
# action
@ from : 
@ to : 
@ quantity : 
@ memo : 
cli push action eosadditapps transfer '["violetstairs", "100.0000 ADDIT", "I LOVE VS"]' -p violetstairs@active
cli push action eosadditapps transfer '["violetstairn", "100.0000 ADDIT", "I LOVE VS"]' -p violetstairn@active

# get table
cli get currency balance violetstairn violetstairn ADDIT
cli get currency balance violetstairn violetstairs ADDIT
```

## burn
> 토큰 소각
```
# action
@ account : 
@ quantity : 
@ memo : 
cli push action eosadditapps burn '["eosadditapps", "10.0000 ADDIT", "I LOVE VS"]' -p eosadditapps@active

# get table
cli get currency balance eosadditapps eosadditapps ADDIT
```



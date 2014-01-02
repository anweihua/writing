-----------------------------------------
--Ԫ������
--TODO�����ַ��������շ���һ���ַ������������ʹ��
--TODO ԭ�����쳣���������������жϣ��߽��жϵȵ�
-----------------------------------------

local print = print
local type = type
local loadstring = loadstring
local math = math
local pairs = pairs
local error = error
module("BaseLib")

local WriteHZ  = nil
local HZStruct = nil
local RunRule = nil
local WZEnv = nil

--�������н��������Ϣ
allInfoStr = ""
retInfoStr = ""

--#####���л������ú���#####--
function clearRetInfoStr()
	retInfoStr = ""
end

function setWriteZiInfo(WZ)
	WriteHZ = WZ
end

function setHZStruct(HS)
	HZStruct = HS
end

function setAllRule(RC)
	RunRule = RC
end


function setWZEnv(env)
	WZEnv = env
end


--#####Ԫ����ʵ��#####--
--���в���������1��ʼ
function GetBH( idx)
	idx = idx + 1
	local bh = WriteHZ.strokes[idx]
	return bh
end

--����Ϊbd��bh����
function GetStartPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[1].x
	point.y = bh.ptSet[1].y
	return point
end

function GetEndPoint(bh)
	local point = WZEnv.POINT:new()
	point.x = bh.ptSet[#bh.ptSet].x
	point.y = bh.ptSet[#bh.ptSet].y
	return point
end

--��ȡ�ʶ�
function GetBDByBH(bh,bdIdx)
	return bh.BDSet[bdIdx]
end

--��ȡ�յ�
function GetInflectionPoint(bh,idx)
	local point = WZEnv.POINT:new()
	local tmpPt = bh.ptSet[bh.InflectionPoint[idx]]
	point.x = tmpPt.x
	point.y = tmpPt.y
	return point
end

--��ñʻ��յ�ĸ���
function GetInflectionPtNum (bh)
	return #bh.InflectionPoint
end

--��ȡ�ʶ�����
 function GetBDNum(bh)
	return #bh.BDSet
 end

 --�����������
 function GetDistance(pt1,pt2)
	local dis = math.sqrt( math.pow(pt1.x - pt2.x,2) + math.pow(pt1.y - pt2.y,2))
	return dis
 end

 function GetBHLen ( bh )
	local nbd = GetBDNum ( bh)
	local bhlen = 0
	--print (nbd)
	for i = 1, nbd do
		local tmpbd = WZEnv.BD:new()
		tmpbd = bh.BDSet[i]
		local tmpspt = GetStartPoint ( tmpbd )
		local tmpept = GetEndPoint ( tmpbd )
		--print (tmpspt.x .. "xx" .. tmpspt.y)
		--print (tmpept.x .. "xx" .. tmpept.y )

		local bdlen = GetDistance ( tmpspt,tmpept )
		bhlen = bhlen + bdlen
	end
	return bhlen
 end

function trace(var)
	retInfoStr = ""
	local varType = type(var)
	if(varType == "string") then
		retInfoStr = var .. "\r\n"
	end
	if(varType == "table") then
			if(var.GeoType == "KBH") then
			retInfoStr = "KBH:\r\n"
			retInfoStr = retInfoStr .. "start point: "..var.ptSet[1].x..","..var.ptSet[1].y
			retInfoStr = retInfoStr .. "\r\n"
			retInfoStr = retInfoStr .. "end point: "..var.ptSet[#var.ptSet].x..","..var.ptSet[#var.ptSet].y
			retInfoStr = retInfoStr .. "\r\n"
			allInfoStr = allInfoStr .. retInfoStr
			end
	end
	if (varType == "number") then
		retInfoStr = var .. "\r\n"
	end
	allInfoStr = allInfoStr .. retInfoStr
end


--����е�
function	GetMidPoint ( bh )
 	local point = WZEnv.POINT:new()
	local len= #bh.ptSet
	local tmpIdx = math.floor ( len / 2 )
	point.x = bh.ptSet[tmpIdx].x
	point.y = bh.ptSet[tmpIdx].y
	return point
end

--��õ�ĺ�������
function GetPointX ( pt )
	return pt.x
end

function GetPointY ( pt )
	return pt.y
end

--����ϡ��bd �Ĳ�ֵ
--[[function base_Interplotation ( bd )
	local resultBD =
	local ptSize = #bd.ptSet
	for i =1 ,ptSize do
		local pt1 = bd.ptSet[i]
		local pt2 = bd.ptSet[i+1]
		local lengthX = pt2.x - pt1.x
		local lengthY = pt2.y - pt2.y
		local maxLength = 0
		if (math.abs (lengthX) > math.abs (lengthY)) then
			maxLength = lengthX
			else
				maxLength = lengthY
			end
		local increaseX = lengthX / maxLength
		local increaseY = lengthY / maxLength
		local startX = pt1.x
		local startY = pt1.y
		for j = 1, j <maxLength
			local point = WZEnv.POINT:new()
			point.x = startX
			point.y = startY

		end
	end
end
]]


--boolean �жϺ��Ƿ�ƽ
function HorFlat ( bh,threshold )
	if (threshold == nil) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH") then
		return nil
	end
	if ( type (threshold) ~= "number" ) then
	end
	local startpt = WZEnv.POINT:new()
	local endpt = WZEnv.POINT:new()
	startpt = bh.ptSet[1]
	endpt = bh.ptSet[#bh.ptSet]
	local slope = (startpt.y - endpt.y) / (endpt.x - startpt.x)
	--lua�з����к������صı������ǽǶȶ����ǻ��ȡ�ע����C++�е�����
	local angel = math.deg ( math.atan(slope))
	local angelInter
	local angelFloat
	angelInter,angelFloat = math.modf (angel)
	if (angelFloat > 0.5) then
		angelInter = angelInter + 1
	elseif (angelFloat < -0.5) then
		angelInter = angelInter - 1
	end
	if (math.abs (angelInter )>= threshold) then
		return false
		else
		return true
	end
end

--boolean �ж����Ƿ�ֱ bh/bd
function VerFlat ( bh,threshold )
	if (threshold == nil) then
	return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH") then
	return nil
	end
	local startpt = WZEnv.POINT:new()
	local endpt = WZEnv.POINT:new()
	startpt = bh.ptSet[1]
	endpt = bh.ptSet[#bh.ptSet]
	if( endpt.y - startpt.y == 0) then
		return nil
	end
	local slope = ( endpt.x - startpt.x ) / ( endpt.y - startpt.y )
	local angel = math.deg ( math.atan(slope))
	local angelInter
	local angelFloat
	angelInter,angelFloat = math.modf (angel)
	if (angelFloat > 0.5) then
		angelInter = angelInter + 1
	elseif (angelFloat < -0.5) then
		angelInter = angelInter - 1
	end
	if (math.abs (angelInter )>= threshold) then
		return 0
		else
		return 1
	end
end

--���������ĵ� bh/bd
function GetLeftMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	if ( bh==nil ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x > bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--���������ĵ� bh/bd
function GetRightMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	if ( bh==nil ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.x < bh.ptSet[i].x ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--������Ϸ��ĵ�
function GetTopMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end
	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.y > bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--������·��ĵ� bd/bd
function GetBottomMostPoint ( bh )
	if ( bh == nil ) then
		return nil
	end
	if ( bh.GeoType ~= "KBD" and bh.GeoType ~= "KBH" ) then
		return nil
	end

	local pt = WZEnv.POINT:new()
	pt.x , pt.y = 0 , 0
	if ( (#bh.ptSet) > 0 ) then
		pt.x = bh.ptSet[1].x
		pt.y = bh.ptSet[1].y
	end
	for i =1,#bh.ptSet do
		if ( pt.y < bh.ptSet[i].y ) then
			pt.x = bh.ptSet[i].x
			pt.y = bh.ptSet[i].y
			end
	end
	return pt
end

--���bh����pt����ĵ�
function GetVPoint(bh,pt)
	local minPtIndex = 0
	local minDis = GetDistance(pt,bh.ptSet[1])
	for i = 2,#bh.ptSet do
		local tmpDis = GetDistance(pt,bh.ptSet[i])
		if (tmpDis < minDis) then
			minDis = tmpDis
			minPtIndex = i
		end
	end
	return bh.ptSet[minPtIndex]
end

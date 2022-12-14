USE [atum2_db_1]
GO
/****** Object:  StoredProcedure [dbo].[Billing_Gift_Item]    Script Date: 21.05.2015 12:27:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
-- =============================================
-- Author:		Fame <-- Is an idiot :P
-- Create date: 20 05 2013
-- Description:	Buy items 
-- =============================================
ALTER PROCEDURE [dbo].[Billing_Gift_Item]
	@i_AccName			VARCHAR(20),
	@i_CharName			VARCHAR(20),
	@i_ItemNum			VARCHAR(50),
	@i_ItemName			VARCHAR(40),
	@i_InsertCount			TINYINT,
	@i_ItemCashPrice			INT,
	@i_AccIP				VARCHAR(20),
	@i_GiftAccUID			INT,
	@i_GiftAccName			VARCHAR(20),
	@i_GiftCharName			VARCHAR(20)
AS
	DECLARE @CashPoint INT
	DECLARE @RemainingCashPoint INT
	DECLARE @GiftCharUID INT

	-- set the remaining cashpoints
	SET @CashPoint = (SELECT CashPoint FROM atum2_db_account.dbo.td_account WHERE AccountName = @i_AccName)
	SET @RemainingCashPoint = @CashPoint - @i_ItemCashPrice
	SET @GiftCharUID = (SELECT [UniqueNumber] FROM [dbo].[td_Character] WHERE [CharacterName] = @i_GiftCharName AND [AccountUniqueNumber] = @i_GiftAccUID)

	--EXEC atum2_db_1.dbo.Billing_Insert_Item @i_GiftAccName, @i_GiftCharName, @i_ItemNum, @i_InsertCount
	-- exec atum2_db_1.dbo.WIKI_InsertStoreItemCountable_DB @i_CharName, @i_ItemNum, @i_InsertCount, 1

	-- decrease the cashpoints
	UPDATE [atum2_db_account].[dbo].[td_Account] SET CashPoint = @RemainingCashPoint WHERE AccountName = @i_AccName

	-- insert in logging table
	INSERT INTO [atum2_db_account].[dbo].[atum_Log_Billing]
		([AccountUID]
		,[AccountName]
		,[CharUID]
		,[CharName]
		,[ItemNum]
		,[ItemName]
		,[InsertCount]
		,[ItemCashPrice]
		,[CashPoint]
		,[RemainingCashPoint]
		,[AccountIP]
		,[GiftAccountUID]
		,[GiftAccountName]
		,[GiftCharUID]
		,[GiftCharName])
	VALUES
		((SELECT TOP 1 [AccountUniqueNumber] FROM [atum2_db_account].[dbo].[td_Account] WHERE [AccountName] = @i_AccName)
		,@i_AccName
		,(SELECT TOP 1 [UniqueNumber] FROM [atum2_db_1].[dbo].[td_Character] WHERE [CharacterName] = @i_CharName)
		,@i_CharName
		,@i_ItemNum
		,@i_ItemName
		,@i_InsertCount
		,@i_ItemCashPrice
		,@CashPoint
		,@RemainingCashPoint
		,@i_AccIP
		,@i_GiftAccUID
		,@i_GiftAccName
		,@GiftCharUID
		,@i_GiftCharName)

	-- return remaining cash point
	SELECT 0, @RemainingCashPoint

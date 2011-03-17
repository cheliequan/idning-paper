var imageWidthLimit = 640;
var imageWidthLimitAnn = 256;
var windowWidth = $(window).width();

function addCommentNum(num) {
	var oldCommentNum = parseInt($("div.entry div.info strong").text());
	oldCommentNum += num;
	$("div.entry div.info strong").text(oldCommentNum.toString());
}

function registerCommentsEvents() {
	// delete comment
	$("#comments input.deleteCommentButton").click(function() {
		var entry_id = $("#hidCurrentEntryId").val();
		if(entry_id == "") {
			alert("未知错误！");
			return;
		}
		
		var cid = $(this).attr("id");
		cid = cid.split("_");
		if(confirm("确定要删除这条评论？")) {
			$.post(g_userSiteRoot + "/mgmt/delete_comment", {
				"hidIsPostBack": "ja",
				"entry_id": entry_id,
				"comment_id": cid[1]
			}, function(rdata) {
				rdata = rdata.split("_");
				if(rdata[0] != "SUCC") {
					alert("Operation failed!");
					return;
				}
				$("#c" + rdata[1]).fadeOut();
				addCommentNum(-1);
			});
		}
	});
}

var slidingOn = false;

function registerCommonEvents() {
	$("div.entry").hover(
		function() {
			$(this).find("div.watch").show();
		},
		function() {
			$(this).find("div.watch").hide();
		}
	);
	
	$("div.entry .watch a").click(function() {
		if(confirm("确定要关注本帖的更新？")) {
			var entryId = $(this).attr("rel").split("_")[1];
			$.post(g_userSiteRoot + "/add_watch", {
				"hidIsPostBack": "ja",
				"entry_id": entryId
			}, function(rdata) {
				rdata = rdata.split("_");
				if(rdata[0] != "SUCC") {
					alert(rdata[1]);
					return;
				}
				alert("已经将本帖添加至你的关注列表。");
			});
		}
	});
	
	// edit entry
	$("div.entry input.editEntryButton").click(function() {
		var entryId = $(this).attr("id").split("_")[1];
		location.href = g_userSiteRoot + "/mgmt/edit_entry/" + entryId;
	});
	
	$("#mgmtBox").hover(
		function() {
			$("#mgmtBox dd").show();
		},
		function() {
			$("#mgmtBox dd").hide();
		}
	);
	
	// popup for friend login
	$("#friendLoginBox").jqm();
	$("#lnkFriendLogin").click(function() {
		$("#friendLoginBox").jqmShow();
		$("#txtFrdPwd").focus();
	});
	$("#friendLoginBox .close").click(function() {
		$("#friendLoginBox").jqmHide();
	});
	
	// popup for prv msg
	$("#prvMsgBox").jqm();
	$("#lnkLeaveMsg").click(function() {
		$("#prvMsgBox").jqmShow();
		$("#txtPMSenderName").focus();
	});
	$("#prvMsgBox .close").click(function() {
		$("#prvMsgBox").jqmHide();
	});
	
	// friend login
	$("#btnFrdPwd").click(function() {
		var frPwd = $("#txtFrdPwd").val();
		if(frPwd != "") {
			$(this).attr("disabled", "disabled");
			$.post(g_userSiteRoot + "/friend_login", {
				"hidIsPostBack": "ja",
				"friend_pwd": frPwd
			}, function(rdata) {
				$("#txtFrdPwd").val("");
				rdata = rdata.split("_");
				if(rdata[0] != "SUCC") {
					alert(rdata[1]);
					$("#btnFrdPwd").removeAttr("disabled");
					return;
				} else {
					$("#btnFrdPwd").removeAttr("disabled");
					location.reload();
				}
			});
		}
	});
	
	// leave msg
	$("#btnSendPM").click(function() {
		var sender_name = $("#txtPMSenderName").val();
		var sender_email = $("#txtPMSenderEMail").val();
		var title = $("#txtPMTitle").val();
		var content = $("#txtPMContent").val();
		if(sender_name == "" || title == "" || content == "") {
			alert("发信人，悄悄话标题和内容都不能为空！");
			return;
		}
		
		var mess = {
			"sender_name": sender_name,
			"sender_email": sender_email,
			"title": title,
			"content": content
		};
		
		$(this).attr("disabled", "disabled");
		
		$.post(g_userSiteRoot + "/leave_message", {
			"hidIsPostBack": "ja",
			"posted_data": JSON.stringify(mess)
		}, function(rdata) {
			$("#btnSendPM").removeAttr("disabled");
			rdata = rdata.split("_");
			if(rdata[0] != "SUCC") {
				alert("Operation failed!");
				return;
			}
			$("#prvMsgBox").jqmHide();
			alert("悄悄话已发送。");
		});
	});
	
	// post comment
	$("#btnPostComment").click(function() {
		var entry_id = $("#hidCurrentEntryId").val();
		if(entry_id == "") {
			alert("未知错误！");
			return;
		}
		
		var author_name = $("#txtAuthorName").val();
		if(author_name != "") {
			if(/[^\u4E00-\u9FA0a-zA-Z0-9._ ]/.test(author_name)) {
				alert("名字中不能含有除中文、字母、数字、句点及下划线之外的字符！");
				return;
			}
		}
		var email = $("#txtEmail").val();
		if(email != "") {
			if(!/\S+[@]{1}\S+[.]{1}\w+/.test(email)) {
				alert("请输入格式正确的 E-mail 地址！");
				return;
			}
		}
		var url = $("#txtUrl").val();
		if(url != "") {
			if(!/http:\/\/\S+[.]{1}\S+/.test(url)) {
				alert("请输入格式正确的网址！");
				return;
			}
		}
		var content = $("#txtCommentContent").val();
		if(content == "") {
			alert("评论内容不能为空！");
			return;
		}
		
		$("#imgWaitComment").show();
		$(this).attr("disabled", "disabled");
		var commentData = {
			"author_name": author_name,
			"email": email,
			"url": url,
			"content": content
		}
		$.post(g_userSiteRoot + "/comment/" + entry_id, {
			"hidIsPostBack": "ja",
			"posted_data": JSON.stringify(commentData)
		}, function(rdata) {
			$("#imgWaitComment").hide();
			$("#btnPostComment").removeAttr("disabled");
			rdata = rdata.split("_");
			if(rdata[0] != "SUCC") {
				alert("Operation failed!");
				return;
			}
			
			$("#txtCommentContent").val("");
			$("#comments_wrapper").load(g_userSiteRoot + "/comments_list/" + entry_id, function() {
				registerCommentsEvents();
				addCommentNum(1);
			});
		});
	});
	
	registerCommentsEvents();
	
	$(window).load(function() {
		$(".entry img.limited").each(function() {
			if($(this).attr("width") > imageWidthLimit) {
				$(this).attr("width", imageWidthLimit);
				$(this).wrap("<a target='_blank' href='" + $(this).attr("src") + "' title='点击查看原图'></a>");
			}
		});
		
		$("#ann img.limited").each(function() {
			if($(this).attr("width") > imageWidthLimitAnn) {
				$(this).attr("width", imageWidthLimitAnn);
				$(this).wrap("<a target='_blank' href='" + $(this).attr("src") + "' title='点击查看原图'></a>");
			}
		});
	});
	
	// only entry-comments page using this
	if(typeof(textAreaAutoExp) != "undefined") {
		if(textAreaAutoExp === true) {
			$("#txtCommentContent").autogrow();
		}
	}
	
	// ie-warning related
	setTimeout(function() {
		$("#ie_warning").hide();
	}, 5000);
}
